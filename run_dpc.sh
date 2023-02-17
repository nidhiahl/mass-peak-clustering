#!/bin/sh

g++ dpc_all_dc.cpp 

<< smallDatasets
commented commnads
./a.out 3C 3 > 3C/dpc_time.csv
python3 three_evaluation_measures.py 3C dpc >> 3C/clusQuality.csv
./a.out 2Q 2 > 2Q/dpc_time.csv
python3 three_evaluation_measures.py 2Q dpc >> 2Q/clusQuality.csv
./a.out 2O 2 > 2O/dpc_time.csv
python3 three_evaluation_measures.py 2O dpc >> 2O/clusQuality.csv
./a.out 3G 3 > 3G/dpc_time.csv
python3 three_evaluation_measures.py 3G dpc >> 3G/clusQuality.csv
./a.out wine 3 > wine/dpc_time.csv
python3 three_evaluation_measures.py wine dpc >> wine/clusQuality.csv
./a.out seeds 3 > seeds/dpc_time.csv
python3 three_evaluation_measures.py seeds dpc >> seeds/clusQuality.csv
./a.out flame 2 > flame/dpc_time.csv
python3 three_evaluation_measures.py flame dpc >> flame/clusQuality.csv
./a.out haberman 2 > haberman/dpc_time.csv
python3 three_evaluation_measures.py haberman dpc >> haberman/clusQuality.csv
./a.out spiral 3 > spiral/dpc_time.csv
python3 three_evaluation_measures.py spiral dpc >> spiral/clusQuality.csv
./a.out ecoli 8 > ecoli/dpc_time.csv
python3 three_evaluation_measures.py ecoli dpc >> ecoli/clusQuality.csv
./a.out liver 2 > liver/dpc_time.csv
python3 three_evaluation_measures.py liver dpc >> liver/clusQuality.csv
./a.out ionosphere 2 > ionosphere/dpc_time.csv
python3 three_evaluation_measures.py ionosphere dpc >> ionosphere/clusQuality.csv
./a.out wdbc 2 > wdbc/dpc_time.csv
python3 three_evaluation_measures.py wdbc dpc >> wdbc/clusQuality.csv
./a.out control 6 > control/dpc_time.csv
python3 three_evaluation_measures.py control dpc >> control/clusQuality.csv
./a.out aggregation 7 > aggregation/dpc_time.csv
python3 three_evaluation_measures.py aggregation dpc >> aggregation/clusQuality.csv
./a.out segment 7 > segment/dpc_time.csv
python3 three_evaluation_measures.py segment dpc >> segment/clusQuality.csv
./a.out D31 31 > D31/dpc_time.csv
python3 three_evaluation_measures.py D31 dpc >> D31/clusQuality.csv
smallDatasets

./a.out sattelite 7 > sattelite/dpc_time.csv
python3 three_evaluation_measures.py sattelite dpc >> sattelite/clusQuality.csv

<<largeDatasets
./a.out thyroid 3 > thyroid/dpc_time.csv
python3 three_evaluation_measures.py thyroid dpc >> thyroid/clusQuality.csv
./a.out smartphone 6 > smartphone/dpc_time.csv
python3 three_evaluation_measures.py smartphone dpc >> smartphone/clusQuality.csv
./a.out pendigits 10 > pendigits/dpc_time.csv
python3 three_evaluation_measures.py pendigits dpc >> pendigits/clusQuality.csv
./a.out shuttle 7 > shuttle/dpc_time.csv
python3 three_evaluation_measures.py shuttle  dpc >> shuttle/clusQuality.csv
./a.out mnist 10 > mnist/dpc_time.csv
python3 three_evaluation_measures.py mnist  dpc >> mnist/clusQuality.csv
./a.out worms_64d 25 > worms_64d/dpc_time.csv
python3 three_evaluation_measures.py worms_64d dpc >> worms_64d/clusQuality.csv
./a.out worms_2d 35 > worms_2d/dpc_time.csv
python3 three_evaluation_measures.py worms_2d dpc >> worms_2d/clusQuality.csv
largeDatasets
