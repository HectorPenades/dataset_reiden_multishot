#include "data_factory_from_reid_multi.h"
#include <stdio.h>
#include <string>
#include <time.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include "boost/lexical_cast.hpp"

using namespace std;
using boost::lexical_cast;

void help()
{
    system("clear");
    cout << "***************************************************************************************************************************" << endl;
    cout << "   -stride [int]" << endl;
    cout << "               int < 0 (random images from cam b ARE NOT chronologically inserted)" << endl;
    cout << "               int = 0 (random images from cam b ARE chronologically inserted)" << endl;
    cout << "               int > 0 (images for persons are consecutively selected with a stride of int)"<< endl;
    cout << "                       (If there are not enough images for one person, an stride between int-1 and 1 is selected)" << endl;
    cout << "   -seed [double] (double denotes a seed for random generation)" << endl;
    cout << "   -number_of_b_samples [int] (int denotes the number of samples for cam_b)" << endl;
    cout << "   -help (shows this menu)" << endl;
    cout << "***************************************************************************************************************************" << endl;
}

void read_initial_parameters (int argc, char **argv, double* seed, int* number_of_b_samples, int* stride, bool* random_b_samples)
{
    *seed=1234;
    *number_of_b_samples = 5;
    *stride = 1;
    *random_b_samples = false;

    for (int i=1; i<argc;i++)
    {
        string aux = lexical_cast<string>(argv[i]); // LEEMOS EL PARAMETRO i
        // SI EL PARAMETRO COINCIDE CON LO QUE BUSCO ("-seed"), ENTONCES GUARDO EL SIGUIENTE PARAMETRO
        // COMO MI VARIABLE (seed = argv[i+1]) transformando al tipo adecuado.
        // Y HAGO i++ PARA EVITAR COMPARAR CON EL VALOR DEL PARAMETRO EN LA SIGUIENTE PASADA DEL BUCLE
        if (aux == "-seed") {*seed = lexical_cast<double>(argv[i+1]); i++;}
        if (aux == "-stride")
        {
            *stride = lexical_cast<int>(argv[i+1]); i++;
            if(*stride <=0 )
                *random_b_samples = true;
            else
                *random_b_samples = false;
        }
        if (aux == "-number_of_b_samples") {*number_of_b_samples = lexical_cast<int>(argv[i+1]); i++;}
        if (aux == "-help") {help(); exit(1);}
    }

    string random_b_samples_s="false";
    if (*random_b_samples) random_b_samples_s="true";
    cout << "***************************************************************************************************************" << endl;
    cout << "* Parameters summary:" << endl;
    cout << "* " << endl;
    cout << "***** random_b_samples = " << random_b_samples_s << endl;
    if (*random_b_samples)
        if (*stride < 0)
            cout << "***** stride = " << *stride << " (Random images from cam b ARE NOT chronologically inserted)" << endl;
            else
                cout << "***** stride = " << *stride << " (Random Images from cam b ARE chronologically inserted)" << endl;
    else
        cout << "***** stride = " << *stride << " (images for persons are consecutively selected with this stride, if possible)" << endl;
    cout << "***** number_of_b_samples = " << *number_of_b_samples << endl;
    cout << "***** seed = " << *seed << endl;
    cout << "***************************************************************************************************************" << endl;

    srand(*seed);
    cout << "Press INTRO to continue..."; // PARA LINUX
    //cin.ignore();  //PARA WINDOWS
    cin.get();
}

void get_multiple_samples(string dataset_folder, int pos , int len, int number_of_b_samples, int stride, bool random_b_samples){
    //SAMPLES FOLDERS
    char samples_folder[150], cam_a_folder[150], cam_b_folder[150];
    strcpy(samples_folder, dataset_folder.c_str());
    strcat(samples_folder, "/multi_shot");
    strcpy(cam_a_folder, samples_folder);
    strcat(cam_a_folder, "/cam_a");
    strcpy(cam_b_folder, samples_folder);
    strcat(cam_b_folder, "/cam_b");

    //LIST OF FILES IN cam_a (random)
    char file_cam_a_name[150];
    strcpy(file_cam_a_name, dataset_folder.c_str());
    strcat(file_cam_a_name, "/DATA/cam_a.txt");
    ofstream file_cam_a(file_cam_a_name);
    if (!file_cam_a.is_open())
        cout<<"1. can not open "<<file_cam_a_name<<endl;
    DIR *dir_a;
    struct dirent *ent_a;
    DIR *dir_a2;
    struct dirent *ent_a2;
    char file_cam_a_person[150];
    vector <string> archivos;
    int posicion;
    vector <string> lines_file_a;

    int random;
    if ((dir_a = opendir (cam_a_folder)) != NULL) {
      	while ((ent_a = readdir (dir_a)) != NULL) {
          	string file= ent_a->d_name;
          	if(file!="." && file!=".."){
              	string label = file.substr (pos,len);

	            strcpy(file_cam_a_person, cam_a_folder);
	    		strcat(file_cam_a_person, "/");
	    		strcat(file_cam_a_person, file.c_str() );

            	if ((dir_a2 = opendir (file_cam_a_person)) != NULL) {
      				while ((ent_a2 = readdir (dir_a2)) != NULL) {
          				 string file2= ent_a2->d_name;
         		 		if(file2!="." && file2!=".."){
         		 			archivos.push_back(file2);
          				}
      				}
      				closedir(dir_a2);
          		}
          		else
          			cout << "5. could not open directory: " << file_cam_a_person << endl;
          		posicion = rand() % archivos.size();
                lines_file_a.push_back("cam_a/"+file+"/"+archivos[posicion] + " "+label);
                archivos.clear();
          	}
      	}
      	int size_lines = lines_file_a.size();
      	for(int i=0; i<size_lines;i++)
        {
            random=rand() % lines_file_a.size();
            file_cam_a<< lines_file_a[random]  << endl;
            lines_file_a.erase(lines_file_a.begin()+random);
        }
        lines_file_a.clear();
        closedir (dir_a);
    } else {
      perror ("2. could not open directory");
    }
    file_cam_a.close();

    //LIST OF FILES IN cam_b (random)
	ofstream file_cam_b_list [number_of_b_samples];
    string file_b_names [number_of_b_samples];
    for (int i=0; i<number_of_b_samples; i++)
    {
    	stringstream ss;
		ss << i+1;
    	file_b_names[i] = dataset_folder + "/DATA/cam_b" + ss.str() + ".txt";
    	cout << file_b_names[i] << endl;
    	file_cam_b_list[i].open(file_b_names[i].c_str());
    	if (!file_cam_b_list[i].is_open())
        	cout<<"3. can not open "<<file_b_names[i]<<endl;
	}

    DIR *dir_b;
    struct dirent *ent_b;
    DIR *dir_b2;
    struct dirent *ent_b2;
    char file_cam_b_person[150];
    vector <string> lines_file_b[number_of_b_samples];
    string images_person [number_of_b_samples];
    string aux;

    if ((dir_b = opendir (cam_b_folder)) != NULL) {
      	while ((ent_b = readdir (dir_b)) != NULL) {
          	string file= ent_b->d_name;
          	if(file!="." && file!=".."){
              	string label = file.substr (pos,len);
	            strcpy(file_cam_b_person, cam_b_folder);
	    		strcat(file_cam_b_person, "/");
	    		strcat(file_cam_b_person, file.c_str() );
            	if ((dir_b2 = opendir (file_cam_b_person)) != NULL) {
      				while ((ent_b2 = readdir (dir_b2)) != NULL) {
          				 string file2= ent_b2->d_name;
         		 		if(file2!="." && file2!=".."){
         		 			archivos.push_back(file2);
          				}
      				}
      				closedir(dir_b2);
          		}
          		else
          			cout << "5. could not open directory: " << file_cam_b_person << endl;

                if(archivos.size()<number_of_b_samples)
                {
                    cout << "There are less images than number_of_b_samples=" << number_of_b_samples << " for person " << file << endl;
                    cout << "Aborting..." << endl;
                    exit(1);
                }
                if(random_b_samples)
                {
                    if(stride<0)
                    {
                        for(int i=0; i<number_of_b_samples ; i++)
                        {
                            posicion = rand() % archivos.size();
                            lines_file_b[i].push_back("cam_b/"+file+"/"+ archivos[posicion] +" "+label);
                            archivos.erase(archivos.begin()+posicion);
                        }
                    }
                    else
                    {
                        for(int i=0; i<number_of_b_samples ; i++)
                        {
                            posicion = rand() % archivos.size();
                            images_person[i]=archivos[posicion];
                            archivos.erase(archivos.begin()+posicion);
                        }
                        for(int i=0; i<number_of_b_samples; i++)
                        {
                            for(int j=0; j < number_of_b_samples-1; j++)
                            {
                                if(images_person[j]>images_person[j+1])
                                 {
                                     aux = images_person[j];
                                     images_person[j]= images_person[j+1];
                                     images_person[j+1] = aux;
                                 }
                            }
                        }
                        for(int i=0; i<number_of_b_samples; i++)
                        {
                            lines_file_b[i].push_back("cam_b/"+file+"/"+ images_person[i] +" "+label);
                        }
                    }
                }
                else
                {
                    int max_ini=0;
                    int stride_aux;
                    for(int i=0; i<stride && max_ini<=0 ; i++)
                    {
                        stride_aux= stride-i;
                        max_ini= archivos.size() - (stride_aux)*(number_of_b_samples-1);
                    }
                    int random1=(rand() % max_ini)+1;
                    string random_string;
                    for (int i=0; i<number_of_b_samples; i++)
                    {
                        random_string = lexical_cast<string>(random1);
                        if(random_string.size()==1)
                        {
                            random_string = "000"+ random_string + ".png";
                        }
                        else if(random_string.size()==2)
                        {
                            random_string = "00"+ random_string + ".png";
                        }
                        else if(random_string.size()==3)
                        {
                            random_string = "0"+ random_string + ".png";
                        }
                        else if(random_string.size()==4)
                        {
                            random_string = random_string + ".png";
                        }

                        lines_file_b[i].push_back("cam_b/"+file+"/"+ random_string +" "+label);
                        random1 = random1 + stride_aux;
                        bool encontrado=false;
                        for(int j=0; j<archivos.size() && encontrado==false; j++)
                        {
                             if(archivos[j]==random_string)
                             {
                                 encontrado=true;
                             }
                        }
                        if(encontrado==false)
                        {
                            cout << "Error processing image " << random_string  << endl;
                            cout << "Image " << random_string << " does not exist in folder " << "cam_b/" << file << "/" << endl << endl;
                            cout << "Aborting..." << endl;
                            exit(1);
                        }
                    }
                }
          		archivos.clear();
          	}
      	}
        int size_lines = lines_file_b[0].size();
        for(int i=0; i<size_lines;i++)
        {
            random=rand() % lines_file_b[0].size();
            for(int j=0; j<number_of_b_samples ; j++)
          		{
          			file_cam_b_list[j]<< lines_file_b[j][random] << endl;
          			lines_file_b[j].erase(lines_file_b[j].begin()+random);
				}
        }
        for(int i=0; i < number_of_b_samples; i++)
        {
              lines_file_b[i].clear();
        }
      closedir (dir_b);
    } else {
      perror ("4. could not open directory");
    }
    for(int i=0; i<number_of_b_samples; i++)
    	file_cam_b_list[i].close();
}

void train_val_test_division_multiple(string dataset_folder, int a_samples_for_training, int b_samples_for_training,  int repeated_samples_for_training, int val_percentage, int a_samples_for_testing, int b_samples_for_testing,  int repeated_samples_for_testing, bool *tag, int number_b_sample){

    char name_number_b_sample[10];
    sprintf(name_number_b_sample,"%d",number_b_sample);  //transforma: int--> char
    int ratio;
    if (val_percentage==0){
        ratio=100+a_samples_for_training+b_samples_for_training;
    }else{
        ratio=int(100/val_percentage);
    }
    char file_cam_a_name[150], file_cam_a_train_name[150], file_cam_a_test_name[150], file_cam_a_val_name[150], file_cam_b_name[150], file_cam_b_train_name[150], file_cam_b_test_name[150], file_cam_b_val_name[150];
    strcpy(file_cam_a_name, dataset_folder.c_str());
    strcat(file_cam_a_name, "/DATA/cam_a.txt");
    ifstream file_cam_a(file_cam_a_name);
    if (!file_cam_a.is_open())
        cout<<"1. can not open "<<file_cam_a_name<<endl;
    strcpy(file_cam_a_train_name, dataset_folder.c_str());
    strcat(file_cam_a_train_name, "/DATA/cam_a_train.txt");

    strcpy(file_cam_a_val_name, dataset_folder.c_str());
    strcat(file_cam_a_val_name, "/DATA/cam_a_val.txt");

    strcpy(file_cam_a_test_name, dataset_folder.c_str());
    strcat(file_cam_a_test_name, "/DATA/cam_a_test.txt");

    strcpy(file_cam_b_name, dataset_folder.c_str());
    strcat(file_cam_b_name, "/DATA/cam_b");
    strcat(file_cam_b_name, name_number_b_sample);
    strcat(file_cam_b_name, ".txt");

    ifstream file_cam_b(file_cam_b_name);
    if (!file_cam_b.is_open())
        cout<<"5. can not open "<<file_cam_b_name<<endl;

    strcpy(file_cam_b_train_name, dataset_folder.c_str());
    strcat(file_cam_b_train_name, "/DATA/cam_b");
    strcat(file_cam_b_train_name, name_number_b_sample);
    strcat(file_cam_b_train_name, "_train.txt");
    ofstream file_cam_b_train(file_cam_b_train_name);
    if (!file_cam_b_train.is_open())
        cout<<"6. can not open "<<file_cam_b_train_name<<endl;

    strcpy(file_cam_b_val_name, dataset_folder.c_str());
    strcat(file_cam_b_val_name, "/DATA/cam_b");
    strcat(file_cam_b_val_name, name_number_b_sample);
    strcat(file_cam_b_val_name, "_val.txt");

    ofstream file_cam_b_val(file_cam_b_val_name);
    if (!file_cam_b_val.is_open())
        cout<<"7. can not open "<<file_cam_b_val_name<<endl;

    strcpy(file_cam_b_test_name, dataset_folder.c_str());
    strcat(file_cam_b_test_name, "/DATA/cam_b");
    strcat(file_cam_b_test_name, name_number_b_sample);
    strcat(file_cam_b_test_name, "_test.txt");

    ofstream file_cam_b_test(file_cam_b_test_name);
    if (!file_cam_b_test.is_open())
        cout<<"8. can not open "<<file_cam_b_test_name<<endl;

    string linea, sample_a, sample_b, label_a, label_b;
    if(*tag==true)
    {
        ofstream file_cam_a_train(file_cam_a_train_name);
        if (!file_cam_a_train.is_open())
            cout<<"2. can not open "<<file_cam_a_train_name<<endl;

        ofstream file_cam_a_val(file_cam_a_val_name);
        if (!file_cam_a_val.is_open())
            cout<<"3. can not open "<<file_cam_a_val_name<<endl;

        ofstream file_cam_a_test(file_cam_a_test_name);
        if (!file_cam_a_test.is_open())
            cout<<"4. can not open "<<file_cam_a_test_name<<endl;

        int count_a_tr=1;
        int count_a_ts=1;
        while(count_a_tr<=a_samples_for_training || count_a_ts<=a_samples_for_testing)
        {
            file_cam_a>> sample_a >> label_a;
            while(getline (file_cam_a,linea)){
                if(count_a_tr <= a_samples_for_training){//search samples for training or validation
                    if(count_a_tr%ratio==0){//validation
                        if(count_a_tr<=repeated_samples_for_training){
                            bool is_repeated=false;
                            ifstream file_cam_b_aux(file_cam_b_name);
                            if (!file_cam_b_aux.is_open())
                                cout<<"can not open "<<file_cam_b_name<<endl;
                            file_cam_b_aux>> sample_b >> label_b;
                            while(getline (file_cam_b_aux,linea)){
                                if(label_a==label_b){
                                    is_repeated=true;
                                }
                                file_cam_b_aux>> sample_b >> label_b;
                            }
                            file_cam_b_aux.close();
                            if(is_repeated){
                                file_cam_a_val<<sample_a<<" "<<label_a<<"\n";
                                count_a_tr++;
                            }else{
                                if(count_a_ts<=a_samples_for_testing){
                                    if(count_a_ts<=repeated_samples_for_testing){
                                        bool is_repeated=false;
                                        ifstream file_cam_b_aux(file_cam_b_name);
                                        if (!file_cam_b_aux.is_open())
                                            cout<<"can not open "<<file_cam_b_name<<endl;
                                        file_cam_b_aux>> sample_b >> label_b;
                                        while(getline (file_cam_b_aux,linea)){
                                            if(label_a==label_b){
                                                is_repeated=true;
                                            }
                                            file_cam_b_aux>> sample_b >> label_b;
                                        }
                                        file_cam_b_aux.close();
                                        if(is_repeated){
                                            file_cam_a_test<<sample_a<<" "<<label_a<<"\n";
                                            count_a_ts++;
                                        }
                                    }else{
                                        file_cam_a_test<<sample_a<<" "<<label_a<<"\n";
                                        count_a_ts++;
                                    }
                                }
                            }
                        }else{
                            file_cam_a_val<<sample_a<<" "<<label_a<<"\n";
                            count_a_tr++;
                        }
                    }else{//training
                        if(count_a_tr<=repeated_samples_for_training){
                            bool is_repeated=false;
                            ifstream file_cam_b_aux(file_cam_b_name);
                            if (!file_cam_b_aux.is_open())
                                cout<<"can not open "<<file_cam_b_name<<endl;
                            file_cam_b_aux>> sample_b >> label_b;
                            while(getline (file_cam_b_aux,linea)){
                                if(label_a==label_b){
                                    is_repeated=true;
                                }
                                file_cam_b_aux>> sample_b >> label_b;
                            }
                            file_cam_b_aux.close();
                            if(is_repeated){
                                file_cam_a_train<<sample_a<<" "<<label_a<<"\n";
                                count_a_tr++;
                            }else{
                                if(count_a_ts<=a_samples_for_testing){
                                    if(count_a_ts<=repeated_samples_for_testing){

                                        bool is_repeated=false;
                                        ifstream file_cam_b_aux(file_cam_b_name);
                                        if (!file_cam_b_aux.is_open())
                                            cout<<"can not open "<<file_cam_b_name<<endl;
                                        file_cam_b_aux>> sample_b >> label_b;
                                        while(getline (file_cam_b_aux,linea)){
                                            if(label_a==label_b){
                                                is_repeated=true;
                                            }
                                            file_cam_b_aux>> sample_b >> label_b;
                                        }
                                        file_cam_b_aux.close();
                                        if(is_repeated){
                                            file_cam_a_test<<sample_a<<" "<<label_a<<"\n";
                                            count_a_ts++;
                                        }
                                    }else{
                                        file_cam_a_test<<sample_a<<" "<<label_a<<"\n";
                                        count_a_ts++;
                                    }
                                }

                            }
                        }else{
                            file_cam_a_train<<sample_a<<" "<<label_a<<"\n";
                            count_a_tr++;
                        }
                    }
                }else{//search samples for testing
                    if(count_a_ts<=a_samples_for_testing){
                        if(count_a_ts<=repeated_samples_for_testing){

                            bool is_repeated=false;
                            ifstream file_cam_b_aux(file_cam_b_name);
                            if (!file_cam_b_aux.is_open())
                                cout<<"can not open "<<file_cam_b_name<<endl;
                            file_cam_b_aux>> sample_b >> label_b;
                            while(getline (file_cam_b_aux,linea)){
                                if(label_a==label_b){
                                    is_repeated=true;
                                }
                                file_cam_b_aux>> sample_b >> label_b;
                            }
                            file_cam_b_aux.close();
                            if(is_repeated){
                                file_cam_a_test<<sample_a<<" "<<label_a<<"\n";
                                count_a_ts++;
                            }
                        }else{
                            file_cam_a_test<<sample_a<<" "<<label_a<<"\n";
                            count_a_ts++;
                        }
                    }
                }
                file_cam_a>> sample_a >> label_a;
            }
            file_cam_a.close();
            file_cam_a.open(file_cam_a_name);
            if (!file_cam_a.is_open())
                cout<<"can not open "<<file_cam_a_name<<endl;
        }
        file_cam_a_train.close();
        file_cam_a_test.close();
        file_cam_a_val.close();
        file_cam_a.close();
        *tag=false;
    }

    int count_b_tr=1;
    int count_b_ts=1;
    while(count_b_tr<=b_samples_for_training || count_b_ts<=b_samples_for_testing){
        file_cam_b>> sample_b >> label_b;
        while(getline (file_cam_b,linea)){
            if(count_b_tr <= b_samples_for_training){//search samples for training
                if(count_b_tr<=repeated_samples_for_training){
                    bool is_repeated=false;
                    ifstream file_cam_a_aux(file_cam_a_train_name);
                    if (!file_cam_a_aux.is_open())
                        cout<<"can not open "<<file_cam_a_train_name<<endl;
                    file_cam_a_aux>> sample_a >> label_a;
                    while(getline (file_cam_a_aux,linea)){
                        if(label_a==label_b){
                            is_repeated=true;
                        }
                        file_cam_a_aux>> sample_a >> label_a;
                    }
                    file_cam_a_aux.close();
                    if(is_repeated){
                        file_cam_b_train<<sample_b<<" "<<label_b<<"\n";
                        count_b_tr++;
                    }else{
                        file_cam_a_aux.open(file_cam_a_val_name);
                        if (!file_cam_a_aux.is_open())
                            cout<<"can not open "<<file_cam_a_val_name<<endl;
                        file_cam_a_aux>> sample_a >> label_a;
                        while(getline (file_cam_a_aux,linea)){
                            if(label_a==label_b){
                                is_repeated=true;
                            }
                            file_cam_a_aux>> sample_a >> label_a;
                        }
                        file_cam_a_aux.close();
                        if(is_repeated){
                            file_cam_b_val<<sample_b<<" "<<label_b<<"\n";
                            count_b_tr++;
                        }else{
                            if(count_b_ts<=b_samples_for_testing){
                                if(count_b_ts<=repeated_samples_for_testing){
                                    bool is_repeated=false;
                                    ifstream file_cam_a_aux(file_cam_a_test_name);
                                    if (!file_cam_a_aux.is_open())
                                        cout<<"can not open "<<file_cam_a_test_name<<endl;
                                    file_cam_a_aux>> sample_a >> label_a;
                                    while(getline (file_cam_a_aux,linea)){
                                        if(label_a==label_b){
                                            is_repeated=true;
                                        }
                                        file_cam_a_aux>> sample_a >> label_a;
                                    }
                                    file_cam_a_aux.close();
                                    if(is_repeated){
                                        file_cam_b_test<<sample_b<<" "<<label_b<<"\n";
                                        count_b_ts++;
                                    }
                                }else{
                                    file_cam_b_test<<sample_b<<" "<<label_b<<"\n";
                                    count_b_ts++;
                                }
                            }
                        }
                    }
                }else{
                    if(count_b_tr%ratio==0){
                        file_cam_b_val<<sample_b<<" "<<label_b<<"\n";
                    }else{
                        file_cam_b_train<<sample_b<<" "<<label_b<<"\n";
                    }
                    count_b_tr++;
                }

            }else{//searching samples for testing
                if(count_b_ts<=b_samples_for_testing){
                    if(count_b_ts<=repeated_samples_for_testing){
                        bool is_repeated=false;
                        ifstream file_cam_a_aux(file_cam_a_test_name);
                        if (!file_cam_a_aux.is_open())
                            cout<<"can not open "<<file_cam_a_test_name<<endl;
                        file_cam_a_aux>> sample_a >> label_a;
                        while(getline (file_cam_a_aux,linea)){
                            if(label_a==label_b){
                                is_repeated=true;
                            }
                            file_cam_a_aux>> sample_a >> label_a;
                        }
                        file_cam_a_aux.close();
                        if(is_repeated){
                            file_cam_b_test<<sample_b<<" "<<label_b<<"\n";
                            count_b_ts++;
                        }
                    }else{
                        file_cam_b_test<<sample_b<<" "<<label_b<<"\n";
                        count_b_ts++;
                    }
                }
            }
            file_cam_b>> sample_b >> label_b;
        }
        file_cam_b.close();
        file_cam_b.open(file_cam_b_name);
        if (!file_cam_b.is_open())
            cout<<"can not open "<<file_cam_b_name<<endl;
    }
    file_cam_b.close();
    file_cam_b_train.close();
    file_cam_b_test.close();
    file_cam_b_val.close();
}

void create_pair_data_multiple(string dataset_folder, int trainset_size, int valset_size, int np, int nn, int number_b_sample){

    char name_number_b_sample[10];
    sprintf(name_number_b_sample,"%d",number_b_sample);  //int--> cadena
    //DATA FOLDER
    char data_folder[150];
    strcpy(data_folder, dataset_folder.c_str());
    strcat(data_folder, "/DATA");
    int e=mkdir(data_folder, ACCESSPERMS);

    //DATA PAIRS FOLDER
    char data_pair_folder[150];
    strcpy(data_pair_folder, data_folder);
    strcat(data_pair_folder, "/PAIR");
    int ePAIR=mkdir(data_pair_folder, ACCESSPERMS);

    //train data sets----------------------------------------------------------------------------------------------------
    char file_cam_a_train_name[150], file_cam_b_train_name[150], file_a_train_name[150], file_b_train_name[150];

    strcpy(file_cam_a_train_name, data_folder);
    strcat(file_cam_a_train_name, "/cam_a_train.txt");
    ifstream file_cam_a_train(file_cam_a_train_name);
    if (!file_cam_a_train.is_open())
        cout<<"1. can not open "<<file_cam_a_train_name<<endl;

    strcpy(file_cam_b_train_name, data_folder);
    strcat(file_cam_b_train_name, "/cam_b");
    strcat(file_cam_b_train_name, name_number_b_sample);
    strcat(file_cam_b_train_name, "_train.txt");
    ifstream file_cam_b_train(file_cam_b_train_name);
    if (!file_cam_b_train.is_open())
        cout<<"2. can not open "<<file_cam_b_train_name<<endl;

    strcpy(file_a_train_name, data_pair_folder);
    strcat(file_a_train_name, "/train_a.txt");
    ofstream file_train_a(file_a_train_name);
    if (!file_train_a.is_open())
        cout<<"3. can not open "<<file_a_train_name<<endl;

    strcpy(file_b_train_name, data_pair_folder);
    strcat(file_b_train_name, "/train_b");
    strcat(file_b_train_name, name_number_b_sample);
    strcat(file_b_train_name, ".txt");

    ofstream file_train_b(file_b_train_name);
    if (!file_train_b.is_open())
        cout<<"4. can not open "<<file_b_train_name<<endl;

    //max id in train_a
    string line;
    int max_id_a=0;
    while ( std::getline (file_cam_a_train,line))//
    {
        std::stringstream ss;
        ss.str(line);
        std::vector<std::string> strs;
        boost::split(strs, line, boost::is_any_of(" "));
        int id=atoi(strs[1].c_str());
        if(id>max_id_a)
            max_id_a=id;
    }
    file_cam_a_train.close();

    //max id in train_b
    int max_id_b=0;
    while ( std::getline (file_cam_b_train,line))//
    {
        std::stringstream ss;
        ss.str(line);
        std::vector<std::string> strs;
        boost::split(strs, line, boost::is_any_of(" "));
        int id=atoi(strs[1].c_str());
        if(id>max_id_b)
            max_id_b=id;
    }
    file_cam_b_train.close();

    file_cam_a_train.open(file_cam_a_train_name);
    if (!file_cam_a_train.is_open())
        cout<<"can not open "<<file_cam_a_train_name<<endl;

    file_cam_b_train.open(file_cam_b_train_name);
    if (!file_cam_b_train.is_open())
        cout<<"can not open "<<file_cam_b_train_name<<endl;

    //vectors WITH THE SAMPLES NAMES train A and B
    vector<string> samples_train_a, samples_train_b;
    samples_train_a.resize(max_id_a);
    samples_train_b.resize(max_id_b);
    while ( std::getline (file_cam_a_train,line))
    {
        std::stringstream ss;
        ss.str(line);
        std::vector<std::string> strs;
        boost::split(strs, line, boost::is_any_of(" "));
        int id=atoi(strs[1].c_str());
        samples_train_a[id-1]=line;
    }
    file_cam_a_train.close();
    while ( std::getline (file_cam_b_train,line))
    {
        std::stringstream ss;
        ss.str(line);
        std::vector<std::string> strs;
        boost::split(strs, line, boost::is_any_of(" "));
        int id=atoi(strs[1].c_str());
        samples_train_b[id-1]=line;
    }
    file_cam_b_train.close();

    //positive and negative matrixes
    vector< vector<string> > positive_train_pairs, negative_train_pairs;
    positive_train_pairs.resize(max_id_a);
    negative_train_pairs.resize(max_id_a * max_id_b);
    for(int i=0; i<max_id_a; i++){
        string sample_a=samples_train_a[i];
        string sample_b=samples_train_b[i];
        if(!sample_a.empty() && !sample_b.empty()){
           vector<string> pair;
           pair.resize(2);
           pair[0]=sample_a;
           pair[1]=sample_b;
           positive_train_pairs[i]=pair;
        }
    }
    for(int a=0; a<max_id_a; a++){
        string sample_a=samples_train_a[a];
        if(!sample_a.empty()){
           for(int b=0; b<max_id_b; b++){
                string sample_b=samples_train_b[b];
                if(!sample_b.empty() && a!=b){
                    vector<string> pair;
                    pair.push_back(sample_a);
                    pair.push_back(sample_b);
                    int idx=a*max_id_b+b;
                    negative_train_pairs[idx]=pair;
                }
           }
        }
    }

    int n_pair=0;
    while(n_pair<trainset_size){
        cout<<n_pair<<endl;
        int p=0;
        while(p<np){
            int randIndex= rand()%min(max_id_a, max_id_b);
            vector<string> pair=positive_train_pairs[randIndex];
            if(!pair.empty()){
                string sample_a=pair[0];
                string sample_b=pair[1];
                file_train_a<<sample_a<<"\n";
                file_train_b<<sample_b<<"\n";
                p++;
                n_pair++;
                cout<<n_pair<<endl;
            }
        }
        int n=0;
        while(n<nn){
            int randIndex= rand()%(max_id_a*max_id_b);
            vector<string> pair=negative_train_pairs[randIndex];
            if(!pair.empty()){
                string sample_a=pair[0];
                string sample_b=pair[1];
                file_train_a<<sample_a<<"\n";
                file_train_b<<sample_b<<"\n";
                n++;
                n_pair++;
                cout<<n_pair<<endl;
            }
        }
        cout<<n_pair<<endl;
    }
    file_train_a.close();
    file_train_b.close();

    //val data sets----------------------------------------------------------------------------------------------------
    char file_cam_a_val_name[150], file_cam_b_val_name[150], file_a_val_name[150], file_b_val_name[150];

    strcpy(file_cam_a_val_name, data_folder);
    strcat(file_cam_a_val_name, "/cam_a_val.txt");
    ifstream file_cam_a_val(file_cam_a_val_name);
    if (!file_cam_a_val.is_open())
        cout<<"1. can not open "<<file_cam_a_val_name<<endl;

    strcpy(file_cam_b_val_name, data_folder);
    strcat(file_cam_b_val_name, "/cam_b");
    strcat(file_cam_b_val_name, name_number_b_sample);
    strcat(file_cam_b_val_name, "_val.txt");

    ifstream file_cam_b_val(file_cam_b_val_name);
    if (!file_cam_b_val.is_open())
        cout<<"2. can not open "<<file_cam_b_val_name<<endl;

    strcpy(file_a_val_name, data_pair_folder);
    strcat(file_a_val_name, "/val_a.txt");
    ofstream file_val_a(file_a_val_name);
    if (!file_val_a.is_open())
        cout<<"3. can not open "<<file_a_val_name<<endl;

    strcpy(file_b_val_name, data_pair_folder);
    strcat(file_b_val_name, "/val_b");
    strcat(file_b_val_name, name_number_b_sample);
    strcat(file_b_val_name, ".txt");
    ofstream file_val_b(file_b_val_name);
    if (!file_val_b.is_open())
        cout<<"4. can not open "<<file_b_val_name<<endl;

    //max id in val_a
    max_id_a=0;
    while ( std::getline (file_cam_a_val,line))//
    {
        std::stringstream ss;
        ss.str(line);
        std::vector<std::string> strs;
        boost::split(strs, line, boost::is_any_of(" "));
        int id=atoi(strs[1].c_str());
        if(id>max_id_a)
            max_id_a=id;
    }
    file_cam_a_val.close();

    //max id in val_b
    max_id_b=0;
    while ( std::getline (file_cam_b_val,line))//
    {
        std::stringstream ss;
        ss.str(line);
        std::vector<std::string> strs;
        boost::split(strs, line, boost::is_any_of(" "));
        int id=atoi(strs[1].c_str());
        if(id>max_id_b)
            max_id_b=id;
    }
    file_cam_b_val.close();

    file_cam_a_val.open(file_cam_a_val_name);
    if (!file_cam_a_val.is_open())
        cout<<"can not open "<<file_cam_a_val_name<<endl;

    file_cam_b_val.open(file_cam_b_val_name);
    if (!file_cam_b_val.is_open())
        cout<<"can not open "<<file_cam_b_val_name<<endl;

    //vectors WITH THE SAMPLES NAMES val A and B
    vector<string> samples_val_a, samples_val_b;
    samples_val_a.resize(max_id_a);
    samples_val_b.resize(max_id_b);
    while ( std::getline (file_cam_a_val,line))
    {
        std::stringstream ss;
        ss.str(line);
        std::vector<std::string> strs;
        boost::split(strs, line, boost::is_any_of(" "));
        int id=atoi(strs[1].c_str());
        samples_val_a[id-1]=line;
    }
    file_cam_a_val.close();
    while ( std::getline (file_cam_b_val,line))
    {
        std::stringstream ss;
        ss.str(line);
        std::vector<std::string> strs;
        boost::split(strs, line, boost::is_any_of(" "));
        int id=atoi(strs[1].c_str());
        samples_val_b[id-1]=line;
    }
    file_cam_b_val.close();

    //positive and negative matrixes
    vector< vector<string> > positive_val_pairs, negative_val_pairs;
    positive_val_pairs.resize(min(max_id_a, max_id_b));
    negative_val_pairs.resize(max_id_a * max_id_b);
    for(int i=0; i<max_id_a; i++){
        string sample_a=samples_val_a[i];
        string sample_b=samples_val_b[i];
        if(!sample_a.empty() && !sample_b.empty()){
           vector<string> pair;
           pair.push_back(sample_a);
           pair.push_back(sample_b);
           positive_val_pairs[i]=pair;
        }
    }
    for(int a=0; a<max_id_a; a++){
        string sample_a=samples_val_a[a];
        if(!sample_a.empty()){
           for(int b=0; b<max_id_b; b++){
                string sample_b=samples_val_b[b];
                if(!sample_b.empty() && a!=b){
                    vector<string> pair;
                    pair.push_back(sample_a);
                    pair.push_back(sample_b);
                    negative_val_pairs[a*max_id_b+b]=pair;
                }
           }
        }
    }

    n_pair=0;
    while(n_pair<valset_size){
        int p=0;
        while(p<np){
            int randIndex= rand()%min(max_id_a, max_id_b);
            vector<string> pair=positive_val_pairs[randIndex];
            if(!pair.empty()){
                string sample_a=pair[0];
                string sample_b=pair[1];
                file_val_a<<sample_a<<"\n";
                file_val_b<<sample_b<<"\n";
                p++;
                n_pair++;
            }
        }
        int n=0;
        while(n<nn){
            int randIndex= rand()%(max_id_a*max_id_b);
            vector<string> pair=negative_val_pairs[randIndex];
            if(!pair.empty()){
                string sample_a=pair[0];
                string sample_b=pair[1];
                file_val_a<<sample_a<<"\n";
                file_val_b<<sample_b<<"\n";
                n++;
                n_pair++;
            }
        }
        cout<<n_pair<<endl;
    }
    file_val_a.close();
    file_val_b.close();
}

void create_pair_data_multiple_remaining(string dataset_folder, int number_b_sample)
{
    char name_number_b_sample[10];
    sprintf(name_number_b_sample,"%d",number_b_sample);  //int--> cadena
    //DATA FOLDER
    char data_folder[150];
    strcpy(data_folder, dataset_folder.c_str());
    strcat(data_folder, "/DATA");
    int e=mkdir(data_folder, ACCESSPERMS);

    //DATA PAIRS FOLDER
    char data_pair_folder[150];
    strcpy(data_pair_folder, data_folder);
    strcat(data_pair_folder, "/PAIR");
    int ePAIR=mkdir(data_pair_folder, ACCESSPERMS);

    //train data sets----------------------------------------------------------------------------------------------------
    char file_bn_train_name[150], file_cam_bn_train_name[150], file_b_train_name[150];

    strcpy(file_b_train_name, data_pair_folder);
    strcat(file_b_train_name, "/train_b1.txt");
    ifstream file_train_b(file_b_train_name);
    if (!file_train_b.is_open())
        cout<<"1. can not open "<<file_b_train_name<<endl;

    strcpy(file_cam_bn_train_name, data_folder);
    strcat(file_cam_bn_train_name, "/cam_b");
    strcat(file_cam_bn_train_name, name_number_b_sample);
    strcat(file_cam_bn_train_name, "_train.txt");
    ifstream file_cam_bn_train(file_cam_bn_train_name);
    if (!file_cam_bn_train.is_open())
        cout<<"2. can not open "<<file_cam_bn_train_name<<endl;

    strcpy(file_bn_train_name, data_pair_folder);
    strcat(file_bn_train_name, "/train_b");
    strcat(file_bn_train_name, name_number_b_sample);
    strcat(file_bn_train_name, ".txt");
    ofstream file_train_bn(file_bn_train_name);
    if (!file_train_bn.is_open())
        cout<<"3. can not open "<<file_bn_train_name<<endl;

    vector <string> sample_bn;
    vector <string> label_bn;
    string linea;
    string sample, label;
    file_cam_bn_train >> sample >> label;
    while(getline (file_cam_bn_train,linea)){
        sample_bn.push_back(sample);
        label_bn.push_back(label);

        file_cam_bn_train >> sample >> label;
    }

    file_train_b >> sample >> label;
    bool finded;
    while(getline (file_train_b,linea)){
        finded = false;
        for(int i=0; i<label_bn.size() && finded == false; i++)
        {
            if(label == label_bn[i])
            {
                finded = true;
                file_train_bn << sample_bn[i] << " "<<label_bn[i] << endl;
            }
        }
        if(finded==false)
            cout << "Something is wrong when building " << file_bn_train_name << endl;

        file_train_b >> sample >> label;
    }

    cout << "File " << file_bn_train_name << " built" << endl;

    file_train_b.close();
    file_cam_bn_train.close();
    file_train_bn.close();
    label_bn.clear();
    sample_bn.clear();

    char file_bn_val_name[150], file_cam_bn_val_name[150], file_b_val_name[150];

    strcpy(file_b_val_name, data_pair_folder);
    strcat(file_b_val_name, "/val_b1.txt");
    ifstream file_val_b(file_b_val_name);
    if (!file_val_b.is_open())
        cout<<"4. can not open "<<file_b_val_name<<endl;

    strcpy(file_cam_bn_val_name, data_folder);
    strcat(file_cam_bn_val_name, "/cam_b");
    strcat(file_cam_bn_val_name, name_number_b_sample);
    strcat(file_cam_bn_val_name, "_val.txt");
    ifstream file_cam_bn_val(file_cam_bn_val_name);
    if (!file_cam_bn_val.is_open())
        cout<<"5. can not open "<<file_cam_bn_val_name<<endl;

    strcpy(file_bn_val_name, data_pair_folder);
    strcat(file_bn_val_name, "/val_b");
    strcat(file_bn_val_name, name_number_b_sample);
    strcat(file_bn_val_name, ".txt");
    ofstream file_val_bn(file_bn_val_name);
    if (!file_val_bn.is_open())
        cout<<"6. can not open "<<file_bn_val_name<<endl;

    file_cam_bn_val >> sample >> label;
    while(getline (file_cam_bn_val,linea)){
        sample_bn.push_back(sample);
        label_bn.push_back(label);

        file_cam_bn_val >> sample >> label;
    }

    file_val_b >> sample >> label;

    while(getline (file_val_b,linea)){
        finded = false;
        for(int i=0; i<label_bn.size() && finded == false; i++)
        {
            if(label == label_bn[i])
            {
                finded = true;
                file_val_bn << sample_bn[i] << " "<<label_bn[i] << endl;
            }
        }
        if(finded==false)
            cout << "Something is wrong when building " << file_bn_val_name << endl;

        file_val_b >> sample >> label;
    }

    cout << "File " << file_bn_val_name << " built" << endl;
    file_val_b.close();
    file_cam_bn_val.close();
    file_val_bn.close();
    label_bn.clear();
    sample_bn.clear();
}


void create_test_data_multiple (string dataset_folder, int number_b_sample, bool *tag){

    char name_number_b_sample[10];
    sprintf(name_number_b_sample,"%d",number_b_sample);

    //DATA FOLDER
    char data_folder[150];
    strcpy(data_folder, dataset_folder.c_str());
    strcat(data_folder, "/DATA");
    int e=mkdir(data_folder, ACCESSPERMS);

    //DATA TEST FOLDER
    char data_test_folder[150];
    strcpy(data_test_folder, data_folder);
    strcat(data_test_folder, "/TEST");
    int etest=mkdir(data_test_folder, ACCESSPERMS );

    char file_cam_a_test_name[150], file_cam_b_test_name[150], file_a_test_name[150], file_b_test_name[150];

    strcpy(file_cam_a_test_name, data_folder);
    strcat(file_cam_a_test_name, "/cam_a_test.txt");
    ifstream file_cam_a_test(file_cam_a_test_name);
    if (!file_cam_a_test.is_open())
        cout<<"1. can not open "<<file_cam_a_test_name<<endl;

    strcpy(file_cam_b_test_name, data_folder);
    strcat(file_cam_b_test_name, "/cam_b");
    strcat(file_cam_b_test_name, name_number_b_sample);
    strcat(file_cam_b_test_name, "_test.txt");
    ifstream file_cam_b_test(file_cam_b_test_name);
    if (!file_cam_b_test.is_open())
        cout<<"2. can not open "<<file_cam_b_test_name<<endl;

    strcpy(file_b_test_name, data_test_folder);
    strcat(file_b_test_name, "/test_b");
    strcat(file_b_test_name, name_number_b_sample);
    strcat(file_b_test_name, ".txt");
    ofstream file_test_b(file_b_test_name);
    if (!file_test_b.is_open())
        cout<<"4. can not open "<<file_b_test_name<<endl;

    string linea, sample_a, sample_b, label_a, label_b;

    int st_a=0;
    while(getline (file_cam_a_test,linea))
        st_a++;
    file_cam_a_test.close();
    file_cam_a_test.open(file_cam_a_test_name);
    if (!file_cam_a_test.is_open())
        cout<<"can not open "<<file_cam_a_test_name<<endl;

    int st_b=0;
    while(getline (file_cam_b_test,linea))
        st_b++;
    file_cam_b_test.close();
    file_cam_b_test.open(file_cam_b_test_name);
    if (!file_cam_b_test.is_open())
        cout<<"can not open "<<file_cam_b_test_name<<endl;

    cout<<"st_a: "<<st_a<<endl;
    cout<<"st_b: "<<st_b<<endl;

    if(*tag==true)
    {
        strcpy(file_a_test_name, data_test_folder);
        strcat(file_a_test_name, "/test_a.txt");
        ofstream file_test_a(file_a_test_name);
        if (!file_test_a.is_open())
            cout<<"3. can not open "<<file_a_test_name<<endl;
        for(int a=0; a<st_a;a++){
            file_cam_a_test>> sample_a >> label_a;
            for(int b=0; b<st_b; b++)
                file_test_a<<sample_a<<" "<<label_a<<"\n";
        }
        *tag=false;
         file_test_a.close();
    }
    for(int a=0; a<st_a;a++){

        for(int b=0; b<st_b; b++){
            file_cam_b_test>> sample_a >> label_a;
            file_test_b<<sample_a<<" "<<label_a<<"\n";
        }
        file_cam_b_test.close();
        file_cam_b_test.open(file_cam_b_test_name);
    }
    file_cam_a_test.close();
    file_cam_b_test.close();
    file_test_b.close();
}
