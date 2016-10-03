/*
 * Copyright (c) 2004-2005 Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * MIT grants permission to use, copy, modify, and distribute this software and
 * its documentation for NON-COMMERCIAL purposes and without fee, provided that
 * this copyright notice appears in all copies.
 *
 * MIT provides this software "as is," without representations or warranties of
 * any kind, either expressed or implied, including but not limited to the
 * implied warranties of merchantability, fitness for a particular purpose, and
 * noninfringement.  MIT shall not be liable for any damages arising from any
 * use of this software.
 *
 * Author: Alexandr Andoni (andoni@mit.edu), Piotr Indyk (indyk@mit.edu)
 */

/*
  The main entry file containing the main() function. The main()
  function parses the command line parameters and depending on them
  calls the correspondin functions.
 */

#include "BasicDefinitions.hpp"
#include "GlobalVars.hpp"
#include "Random.hpp"
#include "Geometry.hpp"
#include "BucketHashing.hpp"
#include "LocalitySensitiveHashing.hpp"
#include "SelfTuning.hpp"
#include "NearNeighbors.hpp"
#include "LSH.hpp"

#define N_SAMPLE_QUERY_POINTS 100

// The data set containing all the points.
PPointT *dataSetPoints = NULL;
// Number of points in the data set.
IntT nPoints = 0;
// The dimension of the points.
IntT pointsDimension = 0;
// The value of parameter R (a near neighbor of a point <q> is any
// point <p> from the data set that is the within distance
// <thresholdR>).
//RealT thresholdR = 1.0;

// The succes probability of each point (each near neighbor is
// reported by the algorithm with probability <successProbability>).
RealT successProbability = 0.9;

// Same as <thresholdR>, only an array of R's (for the case when
// multiple R's are specified).
RealT *listOfRadii = NULL;
IntT nRadii = 0;

RealT *memRatiosForNNStructs = NULL;

PRNearNeighborStructT *nnStructs = NULL;

inline PPointT readPointFromStream(const IntT *dataStream){
  PPointT p;
  RealT sqrLength = 0;
  FAILIF(NULL == (p = (PPointT)MALLOC(sizeof(PointT))));
  FAILIF(NULL == (p->coordinates = dataStream));
  for(IntT d = 0; d < pointsDimension; d++){
    sqrLength += SQR((RealT) p->coordinates[d]);
  }
  p->index = -1;
  p->sqrLength = sqrLength;
  return p;
}

// Reads in the data set points from <memory> in the array
// <dataSetPoints>. Each point get a unique number in the field
// <index> to be easily indentifiable.
void readDataSetFromStream(const IntT *dataStream){
  FAILIF(NULL == (dataSetPoints = (PPointT*)MALLOC(nPoints * sizeof(PPointT))));
  for(IntT i = 0; i < nPoints; i++){
    dataSetPoints[i] = readPointFromStream(&dataStream[i * pointsDimension]);
    dataSetPoints[i]->index = i;
  }
}

// Tranforming <memRatiosForNNStructs> from
// <memRatiosForNNStructs[i]=ratio of mem/total mem> to
// <memRatiosForNNStructs[i]=ratio of mem/mem left for structs i,i+1,...>.
void transformMemRatios(){
  RealT sum = 0;
  for(IntT i = nRadii - 1; i >= 0; i--){
    sum += memRatiosForNNStructs[i];
    memRatiosForNNStructs[i] = memRatiosForNNStructs[i] / sum;
    //DPRINTF("%0.6lf\n", memRatiosForNNStructs[i]);
  }
  ASSERT(sum <= 1.000001);
}


int compareInt32T(const void *a, const void *b){
  Int32T *x = (Int32T*)a;
  Int32T *y = (Int32T*)b;
  return (*x > *y) - (*x < *y);
}

/*
  The function computes the R-NN data structure optimal
  parameters and/or construct the R-NN data structure.
 */      
void LSH_Init(const IntT *dataStream, IntT nElems, IntT nCarac, 
              RealT *radiusList, IntT nRadius){
  // Parse part of the command-line parameters.
  nPoints = nElems;
  pointsDimension = nCarac;

  nRadii = nRadius; 
  ASSERT(nRadii > 0);
  FAILIF(NULL == (listOfRadii = radiusList));
  FAILIF(NULL == (memRatiosForNNStructs = (RealT*)MALLOC(nRadii * sizeof(RealT))));

  printf("computing mem ration for radius\n");
  for(IntT i = 0; i < nRadii; i++){
    ASSERT(listOfRadii[i] > 0);
    memRatiosForNNStructs[i] = 1.0 / nRadii;
    ASSERT(memRatiosForNNStructs[i] > 0);
  }
  DPRINTF("No. radii: %d\n", nRadii);

  if (nPoints > static_cast< int >( MAX_N_POINTS ) ) {
    printf("Error: the structure supports at most %d points (%d were specified).\n", MAX_N_POINTS, nPoints);
    fprintf(ERROR_OUTPUT, "Error: the structure supports at most %d points (%d were specified).\n", MAX_N_POINTS, nPoints);
    exit(1);
  }

  printf("reading data from stream\n");
  readDataSetFromStream(dataStream);
  DPRINTF("Allocated memory (after reading data set): %lld\n", totalAllocatedMemory);

  // Generate a sample query set for computing the optimal parameters.
  Int32T nSampleQueries = N_SAMPLE_QUERY_POINTS;
  PPointT sampleQueries[nSampleQueries];
  Int32T sampleQBoundaryIndeces[nSampleQueries];

  // Choose several data set points for the sample query points.
  printf("selecting samples\n");
  for(IntT i = 0; i < nSampleQueries; i++){
	sampleQueries[i] = dataSetPoints[genRandomInt(0, nPoints - 1)];
  }

  // Compute the array sampleQBoundaryIndeces that specifies how to
  // segregate the sample query points according to their distance
  // to NN.
  printf("sorting samples\n");
  sortQueryPointsByRadii(pointsDimension,
			   nSampleQueries,
			   sampleQueries,
			   nPoints,
			   dataSetPoints,
			   nRadii,
			   listOfRadii,
			   sampleQBoundaryIndeces);

  FAILIF(NULL == (nnStructs = (PRNearNeighborStructT*)MALLOC(nRadii * sizeof(PRNearNeighborStructT))));
  // Determine the R-NN DS parameters, construct the DS and run the queries.
  printf("transforming memory ratios\n");
  transformMemRatios();
  printf("LHS hash functions\n");
  for(IntT i = 0; i < nRadii; i++){
    // XXX: segregate the sample queries...
    nnStructs[i] = initSelfTunedRNearNeighborWithDataSet(listOfRadii[i], 
							   successProbability, 
							   nPoints, 
							   pointsDimension, 
							   dataSetPoints, 
							   nSampleQueries, 
							   sampleQueries, 
							   (MemVarT)((availableTotalMemory - totalAllocatedMemory) * memRatiosForNNStructs[i]));
  }
}


/*
  Runs the queries on the data structure.
 */
IntT LSH_Query(IntT pointIndex, RealT maxRadius, IntT *queryPoints, RealT *queryDistances){
  IntT querySize = 0;

  IntT resultSize = nPoints;
  PPointT *result = (PPointT*)MALLOC(nPoints * sizeof(*result));

  PPointT queryPoint = nullptr; // Fabio: changed here to avoid unitialized value warning diring compilation.
  for(IntT i = 0; i < nPoints; i++){
    if(dataSetPoints[i]->index == pointIndex){
      queryPoint = dataSetPoints[i];
    }	
  }

  // get the near neighbors.
  IntT r = 0;
  while(r < nRadii && listOfRadii[r] < maxRadius){
    r++;	
  }
  IntT nNNs = 0;
  nNNs = getRNearNeighbors(nnStructs[r], queryPoint, result, resultSize);

  if (nNNs > 0){
	// compute the distances to the found NN, and sort according to the distance
    PPointAndRealTStructT *distToNN = NULL;
	FAILIF(NULL == (distToNN = (PPointAndRealTStructT*)MALLOC(nNNs * sizeof(*distToNN))));
	for(IntT p = 0; p < nNNs; p++){
	  distToNN[p].ppoint = result[p];
	  distToNN[p].real = distance(pointsDimension, queryPoint, result[p]);
	}
	qsort(distToNN, nNNs, sizeof(*distToNN), comparePPointAndRealTStructT);

	// report the points
    // FAILIF(NULL == (*queryPoints = (IntT*)MALLOC(nNNs * sizeof(IntT))));	
    // FAILIF(NULL == (*queryDistances = (RealT*)MALLOC(nNNs * sizeof(RealT))));	
    for(IntT j = 1; j < nNNs; j++){
      ASSERT(distToNN[j].ppoint != NULL);
      if(distToNN[j].real <= maxRadius){
        queryPoints[querySize] = distToNN[j].ppoint->index;
        queryDistances[querySize] = distToNN[j].real;
		querySize++;
      }
	}
    FREE(distToNN);
  }
  FREE(result);

  return querySize;
}


void LSH_Free(){
  IntT i;
  for(i = 0; i < nRadii; i++){
    freePRNearNeighborStruct(nnStructs[i]);
  }
  for(i = 0; i < nPoints; i++){
    FREE(dataSetPoints[i]);
  }
  FREE(memRatiosForNNStructs); 
  FREE(dataSetPoints);
  // XXX: should ideally free the other stuff as well.
}

