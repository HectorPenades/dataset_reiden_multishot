# dataset_reiden_multishot

This repository contains a group of functions in C++ for training deep networks with Caffe:

- dataset_reiden_multishot: generation of pairs from Re-Identification datasets with the division of the samples in train, validation and test sets for the multi-shot re-identification problem described in [1]. Additionally, the code allows several strategies for obtaining multiple images from one of the cameras.

- Dataset used: PRID 2011

This functions follows a similar structure of the single-shot code: data_factory_from_reid, avalaible in  https://github.com/magomezs/dataset_factory.


# Citation
Please cite dataset_reiden_multishot in your publications if it helps your research:

[1] José Héctor Penadés-Migallón, Re-identificación de personas a partir de múltiples capturas mediante aprendizaje automático, Trabajo Fin de Grado, Universidad Carlos III de Madrid, July 2020.



# Example of how to use data_factory_from_reid
This is an example of how to use data_factory_from_reid_multi with PRID2011[2]:
 <br />
  <br />

	string prid= "PRID_DATASET_DIRECTORY";  <br />
  read_initial_parameters(argc, argv, &seed, &number_of_b_samples, &stride, &random_b_samples);  <br />
	get_multiple_samples(prid, 7,4, number_of_b_samples,stride, random_b_samples); <br />
	for(int i=1; i<number_of_b_samples+1; i++) <br />
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;train_val_test_division_multiple(prid, 100, 100, 100, 10, 100, 649, 100, &tag, i); <br />
  create_pair_data_multiple(prid, 100000, 10000, 1, 1, 1);    <br />
  for(int i=2; i<number_of_b_samples+1; i++) <br />
      &nbsp;&nbsp;&nbsp;&nbsp; create_pair_data_multiple_remaining(prid, i); <br />
  tag=true; <br />
  for(int i=1; i<number_of_b_samples+1; i++) <br />
     &nbsp;&nbsp;&nbsp;&nbsp;create_test_data_multiple(prid, i, &tag); <br />
   
   
NOTE: be careful with PRID samples whose identification number is higher than 200, because different people in cam a and b are labbelled with the same number, from id 200. Alternative solution: remove samples with ID higher than 200 in cam_a set, they are not neccesarry in the training and test described in [3].

<br />

[2]Person Re-Identification by Descriptive and Discriminative Classification, Martin Hirzer, Csaba Beleznai, Peter M. Roth and Horst Bischof, In Proc. Scandinavian Conference on Image Analysis (SCIA), 2011

[3]Hirzer, M., Beleznai, C., Roth, P. M., and Bischof, H. (2011). Person re-identification by descriptive and
discriminative classification. In Scandinavian conference on Image analysis, pages 91–102. Springer.
    
