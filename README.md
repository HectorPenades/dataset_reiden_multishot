# dataset_reiden_multishot

This repository contains one group of functions in C++

- dataset_reiden_multishot: generation of pairs from Re-Identification datasets with the division of the samples in train, validation and test sets for the multi-shot re-identification problem described in [1]. This code is based on the single-shot code: data_factory_from_reid, avalaible in  https://github.com/magomezs/dataset_factory

The outputs are data txt files with labels, suitable for blobs creation to train deep networks with caffe.

<br />
Then, there is a set of scripts to create the blobs from the datasets using the generated data files (with the previous functions)
- blobs_creation: generate caffe data blobs

<br />

# Citation
Please cite dataset_reiden_multishot in your publications if it helps your research:

[1] José Héctor Penadés-Migallón, Re-identificación de personas a partir de múltiples capturas mediante aprendizaje automático, Trabajo Fin de Grado, Universidad Carlos III de Madrid, Julio 2020.
 
}
.
# Example of how to use data_factory_from_reid
underconstruction
