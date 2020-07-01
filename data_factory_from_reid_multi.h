#ifndef _dataset_factory_from_reid_multi
#define _dataset_factory_from_reid_multi

#include <string>

using namespace std;

void help();
void read_initial_parameters (int , char**, double* , int* , int* , bool* );
void get_multiple_samples(string, int, int, int, int, bool );
void train_val_test_division_multiple(string, int, int, int, int, int, int, int, bool *, int);
void create_pair_data_multiple(string, int, int, int, int, int);
void create_pair_data_multiple_remaining(string , int);
void create_test_data_multiple(string,int,bool*);


#endif
