## Build

```sh
g++ -o dataset_generator dataset_generator.cpp
g++ -o radix_sort radix_sort.cpp -lm
g++ -o radix_sort_step radix_sort_step.cpp -lm
g++ hash_table_search_step.cpp -o hash_table_search_step
g++ hash_table_search.cpp -o hash_table_search
```

## Generate test data

```sh
./dataset_generator 1000
./dataset_generator 10000
./dataset_generator 100000
```

Creates `dataset_1000.csv`, `dataset_10000.csv`, `dataset_100000.csv`.

## Radix Sort

```sh
./radix_sort dataset_1000.csv dataset_10000.csv dataset_100000.csv
```

Performs LSD radix sort on each file, outputs `radix_sorted_dataset_*.csv` with sorted records (integer/string format) and timing info.

## Radix Sort Step-by-Step Trace

```sh
./radix_sort_step dataset_1000.csv 1 7
```

Outputs step-by-step trace of radix sort on rows 1-7. Generates `dataset_1000_radix_sorted_step_1_7.txt` showing array state after each digit pass (d=10 to d=1).


## Hash Table Search Step

```sh
./hash_table_search_step 
```

## Hash Table Search 

```sh
./hash_table_search 
```