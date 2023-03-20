#include <iostream>
#include "./data.cpp"
#include "distance.cpp"
#include "./iforest.cpp"
#include "./mPeakClustering.cpp"


void cc_included_in_top_x_percent(mPeakClustering* refdPObj,const string & dataset){
	ofstream write_ccPosition_in_ordrho(dataset+"/intermediatefiles/mpc_ccPosition_in_ordrho.csv",ios::app|ios::binary);
	if(!write_ccPosition_in_ordrho){
		cout<<"Can not open input data file: intermediatefiles/mpc_ccPosition_in_ordrho.csv"<<endl;
		exit(0);
	}
	double max=0;
	for(auto cc: refdPObj->_clusterCenters){ 	
		for(int i=0;i<refdPObj->_ordrho.size();i++){
			if(cc==refdPObj->_ordrho[i].second && max<i){
				max=i;
			}
		}
	}
	max/=refdPObj->_ordrho.size();
	max*=100;
	write_ccPosition_in_ordrho<<max<<endl;
	write_ccPosition_in_ordrho.close();
	
}


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

	vector<vector<double>> massMatrix;
	iforest *iForestObject = new iforest(refDataObject, 100, 256);
	iforest &refiForestObject = *iForestObject;
	
	iForestObject->constructiForest();
	//cout<<"iforest constructin done"<<endl;
	iForestObject->computeNodeMass();
	//cout<<"nodemass computation done"<<endl;
	iForestObject->computeMassMatrix(massMatrix);
	//cout<<"mass matrix coputation donae"<<endl;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_matrix);
    double matrixTime =  (((end_matrix.tv_sec - start_matrix.tv_sec) * 1e9)+(end_matrix.tv_nsec - start_matrix.tv_nsec))*1e-9;
	//cout<<"MatrixTime="<<matrixTime<<endl;
	
	//cout<<"distance matrix done"<<endl;

//read number of cluster as a user input
	const double k = atof(argv[2]);
	//cout<<"k reading done"<<endl;
//clustering start 
	struct timespec start_sda,end_sda;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_sda);
	
	
	vector<double>sda;
    for(int i = 0; i < dataObj->getnumInstances()-1; i++){
        for(int j = 0; j <= i; j++){
            sda.push_back(massMatrix[i][j]);
        }
    }
    sort(sda.begin(),sda.end());
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_sda);
    double sdaTime =  (((end_sda.tv_sec - start_sda.tv_sec) * 1e9)+(end_sda.tv_nsec - start_sda.tv_nsec))*1e-9;
	


	//cout<<"distances sorting done"<<endl;

	ofstream write_clusterCenters(dataset+"/intermediatefiles/mpc_clusterCenters.csv",ios::out|ios::binary);
	if(!write_clusterCenters){
		cout<<"Can not open input data file: intermediatefiles/mpc_clusterCenters.csv"<<endl;
		exit(0);
	}
	write_clusterCenters.close();

	ofstream write_cId(dataset+"/intermediatefiles/mpc_cId.csv",ios::out|ios::binary);
	if(!write_cId){
		cout<<"Can not open input data file: intermediatefiles/mpc_cId.csv"<<endl;
		exit(0);
	}
	write_cId<<"pointid actual predicted"<<endl;
    write_cId.close();

	ofstream write_ccPosition_in_ordrho(dataset+"/intermediatefiles/mpc_ccPosition_in_ordrho.csv",ios::out|ios::binary);
	if(!write_ccPosition_in_ordrho){
		cout<<"Can not open input data file: intermediatefiles/mpc_ccPosition_in_ordrho.csv"<<endl;
		exit(0);
	}

	write_ccPosition_in_ordrho.close();
	//cout<<"both cid an dhaloid files initialized"<<endl;

	for(double Dc = 0.1; Dc < 100; Dc = Dc+0.1){
		int pointi = 0;
		double _Dc = sda[int(Dc*(sda.size()/100))-1];
		mPeakClustering *mPeakObj = new mPeakClustering(refiForestObject, massMatrix, _Dc); 

//calculate local mass

		struct timespec start_localdensity,end_localdensity;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_localdensity);
		
		mPeakObj->computeLocalMass();

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_localdensity);
		double localdensityTime =  (((end_localdensity.tv_sec - start_localdensity.tv_sec) * 1e9)+(end_localdensity.tv_nsec - start_localdensity.tv_nsec))*1e-9;

		//cout<<"density computation done"<<endl;

//claculate relativedistance
		struct timespec start_relativedistance,end_relativedistance;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_relativedistance);

		mPeakObj->computeRelativeDistance();
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_relativedistance);
    	double relativedistanceTime =  (((end_relativedistance.tv_sec - start_relativedistance.tv_sec) * 1e9)+(end_relativedistance.tv_nsec - start_relativedistance.tv_nsec))*1e-9;
		
	
		//cout<<"delta computation done"<<endl;

//cluster center identification with k

		struct timespec start_ccidentification,end_ccidentification;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_ccidentification);

		mPeakObj->find_k_clustercenters(k);

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_ccidentification);
    
		double ccidentificationTime =  (((end_ccidentification.tv_sec - start_ccidentification.tv_sec) * 1e9)+(end_ccidentification.tv_nsec - start_ccidentification.tv_nsec))*1e-9;


//cout<<"ccidentificationTime="<<ccidentificationTime<<endl;
		
		//ofstream write_clusterCenters(dataset+"/intermediatefiles/mpc_clusterCenters.csv",ios::app|ios::binary);
		
		for(auto i:mPeakObj->_clusterCenters){
			write_clusterCenters<<pointi<<" "<<i<<" "<<refDataObject.dataVector[i]->label<<endl;
			pointi++;
		}
		write_clusterCenters.close();		
		
		//cout<<"cc identification done"<<endl;

		cc_included_in_top_x_percent(mPeakObj,dataset);
	

//cluster assignment
		struct timespec start_clusterassign,end_clusterassign;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_clusterassign);
	
		mPeakObj->clusterAssignment();
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_clusterassign);
		double clusterassignTime =  (((end_clusterassign.tv_sec - start_clusterassign.tv_sec) * 1e9)+(end_clusterassign.tv_nsec - start_clusterassign.tv_nsec))*1e-9;

		//cout<<"cluster assignment done"<<endl;

		ofstream write_cId(dataset+"/intermediatefiles/mpc_cId.csv",ios::app|ios::binary);
		pointi = 0;
		for(auto i:mPeakObj->_cId){
			write_cId<<pointi<<" "<<refDataObject.dataVector[pointi]->label<<" "<<i<<endl;
			pointi++;
		}
		write_cId.close();

		//cout<<"cid writing done"<<endl;

		//cout<<sizeof(massMatrix)<<"+"<<sizeof(mPeakObj)<<" ";
						
		delete mPeakObj;

		cout<<Dc<<" "<<_Dc<<" "<<dPTime<<" "<<matrixTime<<" "<<localdensityTime<<" "<<relativedistanceTime<<" "<<ccidentificationTime<<" "<<clusterassignTime<<endl;
	}































}
