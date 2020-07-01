# dataset_reiden_multishot
 dataset_factory

This repository contains one group of functions in C++

- data_factory_from_reid_multi: generation of pairs from Re-Identification datasets with the division of the samples in train, validation and test sets, according to the protocol described by described in [3].

The outputs are data txt files with labels, suitable for blobs creation to train deep networks with caffe.

<br />
Then, there is a set of scripts to create the blobs from the datasets using the generated data files (with the previous functions)
- blobs_creation: generate caffe data blobs

<br />

# Citation
Please cite dataset_factory_from_reid_multi in your publications if it helps your research:
@article{gomez2019balancing,
  title={“Re-identificación de personas a partir de múltiples capturas mediante aprendizaje automático”},
  author={Penades Migallon, Jose Hector},
  year={2020},
 
}
Gómez-Silva, M. J., Armingol, J. M., & Escalera, A. D. L. (2019). Balancing people re-identification data for deep parts similarity learning. Journal of Imaging Science and Technology, 63(2), 20401-1.
