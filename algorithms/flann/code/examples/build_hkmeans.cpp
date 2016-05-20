#include <flann/flann.hpp>
#include <flann/io/hdf5.h>
#include <getopt.h>
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <sys/time.h>

using namespace flann;
using namespace std;
using std::endl;
using std::ofstream;

float diff_timeval(timeval t1, timeval t2) {
  return (float) (t1.tv_sec - t2.tv_sec) + (t1.tv_usec - t2.tv_usec) * 1e-6;
}

int main(int argc, char** argv)
{

	const struct option longopts[] ={
    {"help",                        no_argument,       0, 'h'},
    {"file_path",                   required_argument, 0, 'f'},
	{"index_path",                  required_argument, 0, 'i'},
    {"branches",                    required_argument, 0, 'b'},
    {"iteration",                   required_argument, 0, 't'},
  };
  	int id;
  	int iarg = 0;
	char file_path[100];
	char index_path[100];
	int branches;
	int iteration;

	while (iarg != -1) {
    iarg = getopt_long(argc, argv, "f:i:t:b:h",longopts, &id);

    switch (iarg) {
      case 'f':
        if (optarg) {
			strcpy(file_path, optarg);
        }
        break;
      case 'i':
        if (optarg) {
			strcpy(index_path, optarg);
        }
        break;
	  case 't':
        if (optarg) {
            iteration = atoi(optarg);
        }
      case 'b':
        if (optarg) {
          branches = atoi(optarg);
        }
        break;
    }
	}

    Matrix<float> dataset;
	load_from_file(dataset, file_path,"dataset");
    
    //build index
    Index<L2<float> > index(dataset, flann::KMeansIndexParams(branches,iteration,FLANN_CENTERS_RANDOM ,0.4));
    clock_t start_time_=clock();
    index.buildIndex();
    double index_time = double(clock()-start_time_)/CLOCKS_PER_SEC;
    cerr<< index_time <<" #index_time "<<endl;                                                               
    index.save(index_path);
 
    delete[] dataset.ptr();
    return 0;
}


