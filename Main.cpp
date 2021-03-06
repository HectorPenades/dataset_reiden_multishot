#include <iostream>
#include "data_factory_from_reid_multi.h"

using namespace std;
int main(int argc, char **argv)
{
	string prid= "PRID_DATASET_DIRECTORY";
	double seed;
	int number_of_b_samples;
   	int stride;
    	bool random_b_samples;
    	bool tag = true;
    	read_initial_parameters(argc, argv, &seed, &number_of_b_samples, &stride, &random_b_samples);
	get_multiple_samples(prid, 7,4, number_of_b_samples,stride, random_b_samples);
	for(int i=1; i<number_of_b_samples+1; i++)
        	train_val_test_division_multiple(prid, 100, 100, 100, 10, 100, 649, 100, &tag, i);
    	create_pair_data_multiple(prid, 100000, 10000, 1, 1, 1);   
    	for(int i=2; i<number_of_b_samples+1; i++)
        	create_pair_data_multiple_remaining(prid, i);
    	tag=true;
    	for(int i=1; i<number_of_b_samples+1; i++)
        	create_test_data_multiple(prid, i, &tag);
}
