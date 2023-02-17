#!/bin/sh

for i in 1 2 3 4 5 6 7 8 9 10
do
	g++ mpc_all_dc.cpp 
<<smallDatasets
	./a.out 3C 3 > 3C/mpc_time.csv
	python3 three_evaluation_measures.py 3C mpc >> 3C/clusQuality.csv
	./a.out 2Q 2 > 2Q/mpc_time.csv
	python3 three_evaluation_measures.py 2Q mpc >> 2Q/clusQuality.csv
	./a.out 2O 2 > 2O/mpc_time.csv
	python3 three_evaluation_measures.py 2O mpc >> 2O/clusQuality.csv
	./a.out 3G 3 > 3G/mpc_time.csv
	python3 three_evaluation_measures.py 3G mpc >> 3G/clusQuality.csv
	./a.out wine 3 > wine/mpc_time.csv
	python3 three_evaluation_measures.py wine mpc >> wine/clusQuality.csv
	./a.out seeds 3 > seeds/mpc_time.csv
	python3 three_evaluation_measures.py seeds mpc >> seeds/clusQuality.csv
	./a.out flame 2 > flame/mpc_time.csv
	python3 three_evaluation_measures.py flame mpc >> flame/clusQuality.csv
	./a.out haberman 2 > haberman/mpc_time.csv
	python3 three_evaluation_measures.py haberman mpc >> haberman/clusQuality.csv
	./a.out spiral 3 > spiral/mpc_time.csv
	python3 three_evaluation_measures.py spiral mpc >> spiral/clusQuality.csv
	./a.out ecoli 8 > ecoli/mpc_time.csv
	python3 three_evaluation_measures.py ecoli mpc >> ecoli/clusQuality.csv
	./a.out liver 2 > liver/mpc_time.csv
	python3 three_evaluation_measures.py liver mpc >> liver/clusQuality.csv
	./a.out ionosphere 2 > ionosphere/mpc_time.csv
	python3 three_evaluation_measures.py ionosphere mpc >> ionosphere/clusQuality.csv
	./a.out wdbc 2 > wdbc/mpc_time.csv
	python3 three_evaluation_measures.py wdbc mpc >> wdbc/clusQuality.csv
	./a.out control 6 > control/mpc_time.csv
	python3 three_evaluation_measures.py control mpc >> control/clusQuality.csv
	./a.out aggregation 7 > aggregation/mpc_time.csv
	python3 three_evaluation_measures.py aggregation mpc >> aggregation/clusQuality.csv
	./a.out segment 7 > segment/mpc_time.csv
	python3 three_evaluation_measures.py segment mpc >> segment/clusQuality.csv

	./a.out D31 31 > D31/mpc_time.csv
	python3 three_evaluation_measures.py D31 mpc >> D31/clusQuality.csv
smallDatasets

	./a.out sattelite 7 > sattelite/mpc_time.csv
	python3 three_evaluation_measures.py sattelite mpc >> sattelite/clusQuality.csv

	
	
	echo "Iteration no $i done"
<<largeDatasets
	./a.out thyroid 3 > thyroid/mpc_time.csv
	python3 three_evaluation_measures.py thyroid mpc >> thyroid/clusQuality.csv
	./a.out smartphone 6 > smartphone/mpc_time.csv
	python3 three_evaluation_measures.py smartphone mpc >> smartphone/clusQuality.csv
	./a.out pendigits 10 > pendigits/mpc_time.csv
	python3 three_evaluation_measures.py pendigits mpc >> pendigits/clusQuality.csv
	./a.out shuttle 7 > shuttle/mpc_time.csv
	python3 three_evaluation_measures.py shuttle  mpc >> shuttle/clusQuality.csv
	./a.out mnist 10 > mnist/mpc_time.csv
	python3 three_evaluation_measures.py mnist  mpc >> mnist/clusQuality.csv
	./a.out worms_64d 25 > worms_64d/mpc_time.csv
	python3 three_evaluation_measures.py worms_64d mpc >> worms_64d/clusQuality.csv
	./a.out worms_2d 35 > worms_2d/mpc_time.csv
	python3 three_evaluation_measures.py worms_2d mpc >> worms_2d/clusQuality.csv
largeDatasets

done
