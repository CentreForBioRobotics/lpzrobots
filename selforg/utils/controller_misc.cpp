#include "controller_misc.h"
#include <algorithm>


using namespace matrix;
using namespace std;

// calculates 1/x
double one_over(double x){
  return 1/x;
}

double constant(double c, double){
  return c;
}

double power(void* c, double x){
  return pow(x,*((double*) c));
}

// creates random number from -1 to 1
double random_minusone_to_one(double){
  return (((double)rand() / RAND_MAX) - 0.5) * 2.0;
}

// creates random number from -1 to 1
double random_minusone_to_one(void* r, double){
  RandGen* g = (RandGen*) r;
  if(!g) return 0;
  else return g->rand()*2 - 1;
}

/// clipping function for mapP
double clip(double r,double x){  
  return clip(x,-r,r);
}

double lowercutof(void* theta, double x){
  return x < *(double*)(theta) ? 0 : x;
}

double toBinaryWithProbability(void* r,double x){
  RandGen* g = (RandGen*) r;
  if (!g) return 0.;
  else return x<(g->rand()*2-1) ? -1. : 1.;
}

double toBinaryWithThreshold(double x, double threshold){
  return x < threshold ? -1. : 1.;
}


double plus_(double b, double a){
  return a + b;
}


/* stores at least left top 4x4 submatrix (row-wise) (if exists) and
   then the rest of the diagonal elements into a list
   @return list of values
*/
list<D> store4x4AndDiagonal(const Matrix& m){
  list<D> l;
  I smalldimM = min(m.getM(), (I)4); // type I is defined in matrix.h
  I smalldimN = min(m.getN(), (I)4);
  I smallerdim = min(m.getM(), m.getN());
  // 4x4
  for(I i=0; i < smalldimM; i++){
    for(I j=0; j < smalldimN; j++){
      l.push_back(m.val(i,j));
    }
  }
  // diagonal below 4x4
  for(I i=4; i < smallerdim; i++){
    l.push_back(m.val(i,i));
  }
  return l;
}

/* stores at least left top 4x4 submatrix (row-wise) (if exists) and
   then the rest of the diagonal elements
  @param len Length of the provided buffer
  (should be min(getN(),4)*min(getM(),4)+ max(0,min(getM()-4,getN()-4)))
  @return number of actually written elements
*/
I store4x4AndDiagonal(const Matrix& m, D* buffer, I len){
  I smalldimM = min(m.getM(), (I)4);
  I smalldimN = min(m.getN(), (I)4);
  I smallerdim = min(m.getM(), m.getN());
  I written=0;
  assert(len >= (I)(smalldimM * smalldimN + max(0, signed(smallerdim) - 4)));
  // 4x4
  for(I i=0; i < smalldimM; i++){
    for(I j=0; j < smalldimN; j++){
      buffer[written]=m.val(i,j);
      written++;
    }
  }
  // diagonal below 4x4
  for(I i=4; i < smallerdim; i++){
    buffer[written]=m.val(i,i);
    written++;
  }
  return written;
}


/* returns the number of elements stored by store4x4AndDiagonal
  (should be min(getN(),4)*min(getM(),4)+ max(0,min(getM()-4,getN()-4)))
*/
I get4x4AndDiagonalSize(const Matrix& m){
  I smalldimM = min(m.getM(), (I)4);
  I smalldimN = min(m.getN(), (I)4);
  I smallerdim = min(m.getM(), m.getN());
  I sm = (I)(max(0, signed(smallerdim) - 4));
  return smalldimM * smalldimN + sm;
}


/* writes the names of the fields stored by store4x4AndDiagonal into a list
  @param matrixName name of the matrix (prefix for all fields)
  @param keylist list for field names
  @param len Length of the provided buffer
  (should be min(getN(),4)*min(getM(),4)+ max(0,min(getM()-4,getN()-4)))
  @return number of actually written elements
*/
list<Inspectable::iparamkey> store4x4AndDiagonalFieldNames(const Matrix& m, const std::string& matrixName){
  list<Inspectable::iparamkey> l;
  char buffer[32];
  I smalldimM = min(m.getM(), (I)4);
  I smalldimN = min(m.getN(), (I)4);
  I smallerdim = min(m.getM(), m.getN());
  // 4x4
  for(I i=0; i < smalldimM; i++){
    for(I j=0; j < smalldimN; j++){
      sprintf(buffer,"%s[%d,%d]",matrixName.c_str(),i,j);
      l.push_back(string(buffer));
    }
  }
  // diagonal below 4x4
  for(I i=4; i < smallerdim; i++){
    sprintf(buffer,"%s[%d,%d]",matrixName.c_str(),i,i);
    l.push_back(string(buffer));
  }
  return l;
}

/* stores the names of the fields stored by store4x4AndDiagonal
  @param matrixName name of the matrix (prefix for all fields)
  @param keylist list for field names
  @param len Length of the provided buffer
  (should be min(getN(),4)*min(getM(),4)+ max(0,min(getM()-4,getN()-4)))
  @return number of actually written elements
*/
I store4x4AndDiagonalFieldNames(const Matrix& m, const std::string& matrixName,
					   char** keylist, I len){
  I smalldimM = min(m.getM(), (I)4);
  I smalldimN = min(m.getN(), (I)4);
  I smallerdim = min(m.getM(), m.getN());
  I written=0;
  assert(len >= get4x4AndDiagonalSize(m));
  unsigned char keyLen = matrixName.length()+10;
  // 4x4
  for(I i=0; i < smalldimM; i++){
    for(I j=0; j < smalldimN; j++){
      keylist[written] = (char*) malloc(keyLen);
      sprintf(keylist[written],"%s[%d,%d]",matrixName.c_str(),i,j);
      written++;
    }
  }
  // diagonal below 4x4
  for(I i=4; i < smallerdim; i++){
    keylist[written] = (char*) malloc(keyLen);
    sprintf(keylist[written],"%s[%d,%d]",matrixName.c_str(),i,i);
    written++;
  }
  return written;
}

/* stores the names of the all matrix fieldnames produces by convertToBuffer into a list
  @return list of names
*/
list<Inspectable::iparamkey> storeMatrixFieldNames(const Matrix& m, const string& matrixName){
  list<Inspectable::iparamkey> l;
  char buffer[32];
  I dimM = m.getM();
  I dimN = m.getN();
  //  assert(matrixName);
  for(I i=0; i < dimM; i++){
    for(I j=0; j < dimN; j++){
      sprintf(buffer,"%s[%d,%d]",matrixName.c_str(),i,j);
      l.push_back(string(buffer));
    }
  }
  return l;
}

/* stores the names of the all vector (mx1 matrix) fieldnames  produces by convertToBuffer into a list
  @return list of names
*/
list<Inspectable::iparamkey> storeVectorFieldNames(const Matrix& m, const string& vectorName){
  list<Inspectable::iparamkey> l;
  char buffer[32];
  I dimM = m.getM();
  //  assert(vectorName);
  assert(m.getN()==1);
  for(I i=0; i < dimM; i++){
      sprintf(buffer,"%s[%d]",vectorName.c_str(),i);
      l.push_back(string(buffer));
  }
  return l;
}

/* stores the names of the all matrix fieldnames produces by convertToBuffer
  @param matrixName name of the matrix (prefix for all fields)
  @param keylist list for field names
  @param len Length of the provided buffer
  (should be getN()*getM()
  @return number of actually written elements
*/
I storeMatrixFieldNames(const Matrix& m, const char* matrixName,
				   char** keylist, I len){
  I dimM = m.getM();
  I dimN = m.getN();
  I written=0;
  assert(matrixName);
  assert(len >= dimM*dimN);
  unsigned char keyLen = strlen(matrixName)+10;
  for(I i=0; i < dimM; i++){
    for(I j=0; j < dimN; j++){
      keylist[written] = (char*) malloc(keyLen);
      sprintf(keylist[written],"%s[%d,%d]",matrixName,i,j);
      written++;
    }
  }
  return written;
}

/* stores the names of the all vector (mx1 matrix) fieldnames produces by convertToBuffer
  @param vectorName name of the vector (prefix for all fields)
  @param keylist list for field names
  @param len Length of the provided buffer (should be getM())
  @return number of actually written elements
*/
I storeVectorFieldNames(const Matrix& m, const char* vectorName,
				   char** keylist, I len){
  I dimM = m.getM();
  I written=0;
  assert(vectorName);
  assert(m.getN()==1);
  assert(len >= dimM);
  unsigned char keyLen = strlen(vectorName)+5;
  for(I i=0; i < dimM; i++){
      keylist[written] = (char*) malloc(keyLen);
      sprintf(keylist[written],"%s[%d]",vectorName,i);
      written++;
  }
  return written;
}



Matrix noiseMatrix(I m, I n, NoiseGenerator& ng,
		   double strength, double unused){
  I len = m*n;
  D* noise = (D*) malloc(len*sizeof(D));
  memset(noise, D_Zero, sizeof(D)*len);
  ng.add(noise, fabs(strength));
  Matrix result(m, n, noise);
  free(noise);
  return result;
}

double matrixNorm1(const matrix::Matrix& m) {
  return m.map(fabs).elementSum() / (m.size());
}

double matrixNorm2(const matrix::Matrix& m) {
  return m.map(sqr).elementSum() / (m.size());
}

double getKthLargestElement(Matrix& vec, I k/*, double* max*/){
  I len = (vec.getM()) * (vec.getN());
  vec.reshape(1,len);
  assert(k>0 && len>=k);
  vec.toSort();
  //  if(max) *max=vec.val(0,len-1);
  return vec.val(0,len-k);
}

double getKthSmallestElement(Matrix& vec, I k/*, double* max*/){
  I len = vec.size();
  vec.reshape(1,len);
  assert(k>0 && len>=k);
  vec.toSort();
  //  if(max) *max=vec.val(0,len-1);
  return vec.val(0,k-1);
}

// considers the matrix as vector (mx1) and returns the index of the smallest element
I argmin(const Matrix& v){
  const double *d = v.unsafeGetData();
  double m = *d;
  I index = 0;
  for(I i=1; i<v.size(); i++){
    if(*(d+i) < m){
      m = *(d+i);
      index = i;
    }
  }
  return index;
}

// considers the matrix as vector (mx1) and returns the index of the largest element
I argmax(const Matrix& v){
  const double *d = v.unsafeGetData();
  double m = *d;
  I index = 0;
  for(I i=1; i<v.size(); i++){
    if(*(d+i) > m){
      m = *(d+i);
      index = i;
    }
  }
  return index;
}

// returns the smallest element
double min(const matrix::Matrix& v){
  return *(std::min_element(v.unsafeGetData(), v.unsafeGetData()+v.size()) );
}


// minimum function for doubles without templates
double min(double a, double b) { 
  return std::min(a,b); 
}

// returns the largest element
double max(const matrix::Matrix& v){
  return *(std::max_element(v.unsafeGetData(), v.unsafeGetData()+v.size()) );
}

// maximum function for doubles without templates
double max(double a, double b) { 
  return std::max(a,b); 
}

// samples from the pdf (rowwise stored with sum = 1)
I sample(const matrix::Matrix& pdf){
  double x = ((double)rand())/(double)RAND_MAX;
  double s=0;
  const double* vs = pdf.unsafeGetData();
  for(I i=0; i<pdf.size(); i++){
    s+=vs[i];
    if(s>=x) return i;
  }
  return pdf.size()-1; // should not happen
}


// parameter adaptation algorithm.
//   @param p current parameter value
//   @param actual actual value of some size controlled by p
//   @param nominal nominal value of some size controlled by p
//   @param up_rate adaptation rate for increasing p (<< 1)
//   @param down_rate adaptation rate for decreasing p (<< 1)
//   @return new value of p (there is no clipping done)
double adapt(double p, double actual, double nominal, double up_rate, double down_rate){
  double result;
  // use a different rate for decreasing and increasing
  result = p * ((actual - nominal) > 0 ? (1.0 - down_rate) : (1.0 + up_rate));
  return result;
}

/// like adapt but that the adaption is just done if the actual value is outside the given interval (_min, _max)
double adaptMinMax(double p, double actual, double _min, double _max, double up_rate, double down_rate){
  double result=p;
  if(actual < _min){
    //    printf("Under: %g, eps: %g\n", updateSize, eps);
    result = p * (1+up_rate);
  } else if(actual > _max){
    //    printf("Over : %g, eps: %g\n", updateSize, eps);
    result = p * (1-down_rate);
  }
  return result;
}

	/**
	 * Helper function for converting an array with double values to a list.
	 * Is used for the method getInternalParams() interface inspectable.
	 */
list<Inspectable::iparamval> convertArrayToList(double* array,int arraySize) {
	std::list<Inspectable::iparamval> l;
	for(int i=0; i < arraySize; i++){
		l.push_back(array[i]);
	}
	return l;
}

	/**
	 * Helper function for getting the array names of an array
	 * Is used for the method getInternalParamNames() interface inspectable.
	 */
list<Inspectable::iparamkey> getArrayNames(int arraySize,const char* name) {
	std::list<Inspectable::iparamkey> nameList;
	char buffer[32];
	for(int i=0; i < arraySize; i++){
		sprintf(buffer,"%s[%d]",name,i);
		nameList.push_back(std::string(buffer));
	}
	return nameList;
}
