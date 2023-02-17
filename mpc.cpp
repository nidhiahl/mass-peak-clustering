//./a.out 

#include <iostream>
#include "./data.cpp"
#include "distance.cpp"
#include "./iforest.cpp"
#include "./dPeakClustering.cpp"

int main(int argc, char* argv[])  //(argv[1] = inputdataFile.csv , argv[2] = cutoff distance , argv[3] = number of clusters
{
	srand(time(0));
//read input from csv file

	const string dataFile = argv[1];
	
	
	data *dataObj = new data();
    dataObj->createDataVector(dataFile);
    const data &refDataObject = *dataObj;
	
	cout<<"data read done"<<endl;



//claculate mass matrix

	vector<vector<double>> massMatrix;
	iforest *iForestObject = new iforest(refDataObject, 100, 256);
	const iforest &refiForestObject = *iForestObject;
	iForestObject->constructiForest();
	iForestObject->computeNodeMass();
	iForestObject->computeMassMatrix(massMatrix);	

    cout<<"distance matrix done"<<endl;


//Clustering start

	const double Dc = atof(argv[2]);
	dPeakClustering *dPeakObj = new dPeakClustering(massMatrix, refiForestObject, Dc, "Cut-off");  //replace "Cut-off" with "Gaussian" for gaussian density type
	cout<<"Dc reading done"<<endl;
	

//claculate localdensity
	
	dPeakObj->computeLocalMass();
	
	cout<<"density computation done"<<endl;
//claculate relativedistance
	
	dPeakObj->computeRelativeDistance();
	

	ofstream write_ordrho("intermediatefiles/ordrho.csv",ios::out|ios::binary);
	if(!write_ordrho){
		cout<<"Can not open input data file: intermediatefiles/ordrho.csv"<<endl;
		exit(0);
	}
	for(int i=0;i<dPeakObj->_ordrho.size();i++){
		write_ordrho<<dPeakObj->_ordrho[i].second<<" "<<dPeakObj->_ordrho[i].first<<endl;
	}
	write_ordrho.close();


	ofstream write_delta("intermediatefiles/delta.csv",ios::out|ios::binary);
	if(!write_delta){
		cout<<"Can not open input data file: intermediatefiles/delta.csv"<<endl;
		exit(0);
	}
	int pointi = 0;
	for(auto i:dPeakObj->_delta){
		write_delta<<pointi<<" "<<i<<endl;
		pointi++;
	}
	write_delta.close();
	


	ofstream write_rhodelta("intermediatefiles/rhodelta.csv",ios::out|ios::binary);
	if(!write_rhodelta){
		cout<<"Can not open input data file: intermediatefiles/rhodelta.csv"<<endl;
		exit(0);
	}
	for(int i=0;i<dPeakObj->_rhodelta.size();i++){
		write_rhodelta<<dPeakObj->_rhodelta[i].second<<" "<<dPeakObj->_rhodelta[i].first<<endl;
	}
	write_rhodelta.close();




	cout<<"delta computation done"<<endl;



//cluster center identification
	const double k = atof(argv[3]);

	dPeakObj->find_k_clustercenters(k);
	

	ofstream write_clusterCenters("intermediatefiles/clusterCenters.csv",ios::out|ios::binary);
	if(!write_clusterCenters){
		cout<<"Can not open input data file: intermediatefiles/clusterCenters.csv"<<endl;
		exit(0);
	}
	pointi = 0;
	for(auto i:dPeakObj->_clusterCenters){
		write_clusterCenters<<pointi<<" "<<i<<endl;
		pointi++;
	}
	write_clusterCenters.close();



	cout<<"cc identification done"<<endl;
//cluster assignment
	dPeakObj->clusterAssignment();
	

	ofstream write_cId("intermediatefiles/cId.csv",ios::out|ios::binary);
	if(!write_cId){
		cout<<"Can not open input data file: intermediatefiles/cId.csv"<<endl;
		exit(0);
	}
	pointi = 0;
	write_cId<<"pointid actual predicted"<<endl;
	for(auto i:dPeakObj->_cId){
		write_cId<<pointi<<" "<<refDataObject.dataVector[pointi]->label<<" "<<i<<endl;
		pointi++;
	}
	write_cId.close();


	cout<<"cluster assignment done"<<endl;
//halo finding in each cluster

	
	dPeakObj->findHalo();
	
	ofstream write_haloId("intermediatefiles/haloId.csv",ios::out|ios::binary);
	if(!write_haloId){
		cout<<"Can not open input data file: intermediatefiles/haloId.csv"<<endl;
		exit(0);
	}
	write_haloId<<"pointid actual predicted"<<endl;
	pointi = 0;
	for(auto i:dPeakObj->_haloId){
		write_haloId<<pointi<<" "<<refDataObject.dataVector[pointi]->label<<" "<<i<<endl;
		pointi++;
	}
	write_haloId.close();	

	cout<<"Halo finding done"<<endl;
	return 0;
}















