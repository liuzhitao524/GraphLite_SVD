# GraphLite_SVD
------------------------------------------------------------
Compile and Run Vertex Program
------------------------------------------------------------

1. build svd

   $ cd svd
   $ make

   check if svd/SVD.so is successfully generated.
   
2. run example

   $ start-graphlite svd/SVD.so input/matrix_4w Output/out 5 5

   SVD.cc declares 5 processes, including 1 master and 4 workers.
   So the input graph file is prepared as four files: input/matrix_4w_[1-4]

   The output of PageRank will be in: Output/out_[1-4]

   Workers generate log files in WorkOut/worker*.out
3.svd diretectory : source code
  input directory :test data
------------------------------------------------------------
4. hash partioner
   input/split can be used to divide a graph input file into multiple partitions 
   #./slit
   will generate :input/matrix_4w_[1-4]	
