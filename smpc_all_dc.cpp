#include <iostream>
#include "./data.cpp"
#include "distance.cpp"
#include "./iforest.cpp"
#include "./mPeakClustering.cpp"

int main(int argc, char* argv[])      //(argv[1] = inputdataFile.csv , argv[2] = number of clusters
{
	srand(time(0));
//read input from csv file
	cout<<"Dc dPTime iforestTime find_markedNodesTime find_potential_dcNN_listTime  relativedistanceTime ccidentificationTime_without_k ccidentificationTime clusterassignTime "<<endl;
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


//claculate iforest construction

	
	struct timespec start_iforest,end_iforest;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_iforest);
	vector<vector<double>> massMatrix;
	
	iforest *iForestObject = new iforest(refDataObject, 100, 256);
	iforest &refiForestObject = *iForestObject;
	
	iForestObject->constructiForest();
	iForestObject->computeNodeMass();
	iForestObject->computeLCA_lookup();
	
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_iforest);
    double iforestTime =  (((end_iforest.tv_sec - start_iforest.tv_sec) * 1e9)+(end_iforest.tv_nsec - start_iforest.tv_nsec))*1e-9;
	//cout<<"MatrixTime="<<iforestTime<<endl;
	
	//cout<<"iforest done"<<endl;
	iForestObject->write_smallest_largest_leaf(dataset);

//read number of cluster as a user input
	const double k = atof(argv[2]);
	//cout<<"k reading done"<<endl;



//clustering start 
	
	ofstream write_clusterCenters(dataset+"/intermediatefiles/smpc_clusterCenters.csv",ios::out|ios::binary);
	if(!write_clusterCenters){
		cout<<"Can not open input data file: intermediatefiles/smpc_clusterCenters.csv"<<endl;
		exit(0);
	}
	write_clusterCenters.close();

	ofstream write_cId(dataset+"/intermediatefiles/smpc_cId.csv",ios::out|ios::binary);
	if(!write_cId){
		cout<<"Can not open input data file: intermediatefiles/smpc_cId.csv"<<endl;
		exit(0);
	}
	write_cId<<"pointid actual predicted"<<endl;
    write_cId.close();



	//cout<<"both cid an dhaloid files initialized"<<endl;

	for(double Dc = 0.001; Dc < 1; Dc = Dc+0.001){
		int pointi = 0;
		//cout<<"Dc="<<Dc<<endl;
		//cout<<"sda[]="<<int(Dc*(sda.size()/100))-1<<endl;
		//cout<<"sda[]="<<int(Dc*((dataObj->getnumInstances()-1)*(dataObj->getnumInstances()-2)/2)/100)-1<<endl;
		//cout<<"_Dc="<<_Dc<<endl;
		mPeakClustering *mPeakObj = new mPeakClustering(refiForestObject, massMatrix, Dc);  
		//cout<<"mPeakobject created"<<endl;
	
//find marked nodes

		struct timespec start_find_markedNodes,end_find_markedNodes;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_find_markedNodes);
		
	
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_find_markedNodes);
		double find_markedNodesTime =  (((end_find_markedNodes.tv_sec - start_find_markedNodes.tv_sec) * 1e9)+(end_find_markedNodes.tv_nsec - start_find_markedNodes.tv_nsec))*1e-9;
		//cout<<"find_markedNodesTime="<<find_markedNodesTime<<endl;

		//cout<<"find_markedNodes done"<<endl;


//find potential mass-based neighbors(PMBN)

		struct timespec start_find_potential_dcNN_list,end_find_potential_dcNN_list;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_find_potential_dcNN_list);
		
		mPeakObj->find_potential_dcNN_list();

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_find_potential_dcNN_list);
		double find_potential_dcNN_listTime =  (((end_find_potential_dcNN_list.tv_sec - start_find_potential_dcNN_list.tv_sec) * 1e9)+(end_find_potential_dcNN_list.tv_nsec - start_find_potential_dcNN_list.tv_nsec))*1e-9;
		//cout<<"find_potential_dcNN_listTime="<<find_potential_dcNN_listTime<<endl;

		//cout<<"find_potential_dcNN_list done"<<endl;



	//claculate relative distance
		struct timespec start_relativedistance,end_relativedistance;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_relativedistance);

		mPeakObj->sRelativeDistance();
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_relativedistance);
    	double relativedistanceTime =  (((end_relativedistance.tv_sec - start_relativedistance.tv_sec) * 1e9)+(end_relativedistance.tv_nsec - start_relativedistance.tv_nsec))*1e-9;
		//cout<<"relativedistanceTime="<<relativedistanceTime<<endl;
	
		//cout<<"delta computation done"<<endl;


//clustercenter identification without k
		struct timespec start_ccidentification_without_k,end_ccidentification_without_k;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_ccidentification_without_k);

		//mPeakObj->numberOfClusterCenters();

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_ccidentification_without_k);
    
		double ccidentificationTime_without_k =  (((end_ccidentification_without_k.tv_sec - start_ccidentification_without_k.tv_sec) * 1e9)+(end_ccidentification_without_k.tv_nsec - start_ccidentification_without_k.tv_nsec))*1e-9;

		/*ofstream write_clusterCenters(dataset+"/intermediatefiles/smpc_clusterCenters.csv",ios::app|ios::binary);
		
		for(auto i:mPeakObj->_clusterCenters){
			write_clusterCenters<<"--"<<pointi<<" "<<i<<" "<<refDataObject.dataVector[i]->label<<endl;
			pointi++;
		}*/
		//write_clusterCenters.close();		

	//cout<<"delta computation done"<<endl;
	//cout<<"cc identification done"<<endl;

	//cluster center identification with k

		struct timespec start_ccidentification,end_ccidentification;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_ccidentification);

		mPeakObj->assignClusterId(k);

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

	//cluster assignment
		struct timespec start_clusterassign,end_clusterassign;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_clusterassign);
	
		//mPeakObj->clusterAssignment();
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_clusterassign);
		double clusterassignTime =  (((end_clusterassign.tv_sec - start_clusterassign.tv_sec) * 1e9)+(end_clusterassign.tv_nsec - start_clusterassign.tv_nsec))*1e-9;
		//cout<<"clusterassignTime="<<clusterassignTime<<endl;


		//cout<<"cluster assignment done"<<endl;

		ofstream write_cId(dataset+"/intermediatefiles/smpc_cId.csv",ios::app|ios::binary);
		pointi = 0;
		for(auto i:mPeakObj->_cId){
			write_cId<<pointi<<" "<<refDataObject.dataVector[pointi]->label<<" "<<i<<endl;
			pointi++;
		}
		write_cId.close();

		//cout<<"cid writing done"<<endl;
	
		//cout<<sizeof(massMatrix)<<"+"<<sizeof(mPeakObj)<<" ";
						
		delete mPeakObj;

		cout<<Dc<<" "<<dPTime<<" "<<iforestTime<<" "<<find_markedNodesTime<<" "<<find_potential_dcNN_listTime<<" "<<relativedistanceTime<<" "<<ccidentificationTime_without_k<<" "<<ccidentificationTime<<" "<<clusterassignTime<<endl;
	}































}
