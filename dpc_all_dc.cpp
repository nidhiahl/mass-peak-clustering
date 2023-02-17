#include <iostream>
#include "./data.cpp"
#include "distance.cpp"
#include "./iforest.cpp"
#include "./dPeakClustering.cpp"

int main(int argc, char* argv[])      //(argv[1] = inputdataFile.csv , argv[2] = number of clusters
{
	srand(time(0));
//read input from csv file
	cout<<"Dc(%) Dc(distance) dPTime matrixTime localdensityTime relativedistanceTime ccidentificationTime clusterassignTime"<<endl;
	const string dataset = argv[1];
	
	struct timespec start_dP,end_dP;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_dP);

	data *dataObj = new data();
    dataObj->createDataVector(dataset+"/"+dataset+".csv");
    const data &refDataObject = *dataObj;
	
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_dP);
    double dPTime =  (((end_dP.tv_sec - start_dP.tv_sec) * 1e9)+(end_dP.tv_nsec - start_dP.tv_nsec))*1e-9;

	//cout<<"dPTime="<<dPTime<<endl;
	
	//cout<<"data read done"<<endl;


//claculate distance matrix

	
	struct timespec start_matrix,end_matrix;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_matrix);

	vector<vector<double>> distMatrix;
	
	Distance *distObj = new Distance(refDataObject,"Euclidean");

    distObj->computeDistanceMatrix(distMatrix);
	
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_matrix);
    double matrixTime =  (((end_matrix.tv_sec - start_matrix.tv_sec) * 1e9)+(end_matrix.tv_nsec - start_matrix.tv_nsec))*1e-9;
	
	//cout<<"distance matrix done"<<endl;

//read number of cluster as a user input
	const double k = atof(argv[2]);
	//cout<<"k reading done"<<endl;
//clustering start 

	vector<double>sda;
    for(int i = 0; i < dataObj->getnumInstances()-1; i++){
        for(int j = 0; j < i; j++){
            sda.push_back(distMatrix[i][j]);
        }
    }
    sort(sda.begin(),sda.end());
	//cout<<"distances sorting done"<<endl;

	ofstream write_clusterCenters(dataset+"/intermediatefiles/dpc_clusterCenters.csv",ios::out|ios::binary);
	if(!write_clusterCenters){
		cout<<"Can not open input data file: intermediatefiles/clusterCenters.csv"<<endl;
		exit(0);
	}
	write_clusterCenters.close();

	ofstream write_cId(dataset+"/intermediatefiles/dpc_cId.csv",ios::out|ios::binary);
	if(!write_cId){
		cout<<"Can not open input data file: intermediatefiles/dpc_cId.csv"<<endl;
		exit(0);
	}
	write_cId<<"pointid actual predicted"<<endl;
    write_cId.close();



	//cout<<"both cid an dhaloid files initialized"<<endl;

	for(double Dc = 0.1; Dc < 100; Dc = Dc+0.1){
		int pointi = 0;

		double _Dc = sda[int(Dc*(sda.size()/100))-1];

		dPeakClustering *dPeakObj = new dPeakClustering(distMatrix, _Dc);  //replace "Cut-off" with "Gaussian" for gaussian density type
		
//calculate local density

		struct timespec start_localdensity,end_localdensity;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_localdensity);
		
		dPeakObj->computeLocalDensity();

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_localdensity);
		double localdensityTime =  (((end_localdensity.tv_sec - start_localdensity.tv_sec) * 1e9)+(end_localdensity.tv_nsec - start_localdensity.tv_nsec))*1e-9;
		
		//cout<<"density computation done"<<endl;

//claculate relativedistance
		struct timespec start_relativedistance,end_relativedistance;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_relativedistance);

		dPeakObj->computeRelativeDistance();
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_relativedistance);
    	double relativedistanceTime =  (((end_relativedistance.tv_sec - start_relativedistance.tv_sec) * 1e9)+(end_relativedistance.tv_nsec - start_relativedistance.tv_nsec))*1e-9;
		
		//cout<<"delta computation done"<<endl;


//cluster center identification with k

		struct timespec start_ccidentification,end_ccidentification;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_ccidentification);

		dPeakObj->find_k_clustercenters(k);

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_ccidentification);
    
		double ccidentificationTime =  (((end_ccidentification.tv_sec - start_ccidentification.tv_sec) * 1e9)+(end_ccidentification.tv_nsec - start_ccidentification.tv_nsec))*1e-9;
		
		ofstream write_clusterCenters(dataset+"/intermediatefiles/dpc_clusterCenters.csv",ios::app|ios::binary);
		
		for(auto i:dPeakObj->_clusterCenters){
			write_clusterCenters<<pointi<<" "<<i<<" "<<refDataObject.dataVector[i]->label<<endl;
			pointi++;
		}
		write_clusterCenters.close();		
		
		//cout<<"cc identification done"<<endl;

//cluster assignment
		struct timespec start_clusterassign,end_clusterassign;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_clusterassign);
	
		dPeakObj->clusterAssignment();
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_clusterassign);
		double clusterassignTime =  (((end_clusterassign.tv_sec - start_clusterassign.tv_sec) * 1e9)+(end_clusterassign.tv_nsec - start_clusterassign.tv_nsec))*1e-9;

		//cout<<"cluster assignment done"<<endl;

		ofstream write_cId(dataset+"/intermediatefiles/dpc_cId.csv",ios::app|ios::binary);
		pointi = 0;
		for(auto i:dPeakObj->_cId){
			write_cId<<pointi<<" "<<refDataObject.dataVector[pointi]->label<<" "<<i<<endl;
			pointi++;
		}
		write_cId.close();

		//cout<<"cid writing done"<<endl;

		//cout<<sizeof(distMatrix)<<"+"<<sizeof(dPeakObj)<<" ";
		delete dPeakObj;

		cout<<Dc<<" "<<_Dc<<" "<<dPTime<<" "<<matrixTime<<" "<<localdensityTime<<" "<<relativedistanceTime<<" "<<ccidentificationTime<<" "<<clusterassignTime<<endl;
	
	}































}
