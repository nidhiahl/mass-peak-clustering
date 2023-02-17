#pragma once
#include "data.h"
#include "distance.h"
#include "data.cpp"
#include "distance.cpp"
#include "localDensity.h"
#include "localDensity.cpp"
#include "dPeakClustering.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>


class StdDevCalc{
    private:

        double count;
        double meanPrev, meanCurr;
        double sPrev, sCurr;
        double varianceCurr;

    public:

        StdDevCalc() {
            count = 0;
        }

        void append(double d) {
            count++;

            if (count == 1) {
                // Set the very first values.
                meanCurr     = d;
                sCurr        = 0;
                varianceCurr = sCurr;
            }
            else {
                // Save the previous values.
                meanPrev     = meanCurr;
                sPrev        = sCurr;

                // Update the current values.
                meanCurr     = meanPrev + (d - meanPrev) / count;
                sCurr        = sPrev    + (d - meanPrev) * (d - meanCurr);
                varianceCurr = sCurr / (count - 1);
            }
        }

    double get_std_dev() {
        return sqrt(varianceCurr);
    }
};






dPeakClustering :: dPeakClustering(const vector<vector<double>> & distMatrix, const double & Dc): _distMatrix(distMatrix), _Dc(Dc){
	_totalPoints = _distMatrix.size();
	_maxDistance = 0;
}


dPeakClustering::~dPeakClustering(){}


void dPeakClustering :: computeLocalDensity(){
	 _rho.assign(_totalPoints,1);    
        for(int i = 0; i < _totalPoints-1; i++){
            for(int j = 0; j < i; j++){
				_maxDistance = max(_maxDistance, _distMatrix[i][j]);
                if (_distMatrix[i][j] < _Dc){
                    _rho[i]++;
                    _rho[j]++;
                }
            }
        }

}



// function to compute delta and nearest neighbour for every point 
void dPeakClustering:: computeRelativeDistance(){
	for(int i = 0; i < _totalPoints; i++){
        _ordrho.push_back({_rho[i],i});
    }
	sort(_ordrho.rbegin(),_ordrho.rend());
	_delta.resize(_totalPoints);
    _nneigh.resize(_totalPoints);
	_delta[_ordrho[0].second] = _maxDistance;
	_nneigh[_ordrho[0].second] = -1;
    _rhodelta.push_back({_ordrho[0].first * _delta[_ordrho[0].second],_ordrho[0].second});
	double secondMax_rhodelta = 0;
    for(int i = 1; i < _totalPoints; i++){
		_delta[_ordrho[i].second] =_maxDistance;
        _nneigh[_ordrho[i].second] = i;
		double min = _maxDistance;
		int minNN=-2;
		for(int j = 0;j < i; j++){
		double dis = _ordrho[i].second > _ordrho[j].second?_distMatrix[_ordrho[i].second][_ordrho[j].second]:_distMatrix[_ordrho[j].second][_ordrho[i].second];
		if(dis < min){
				min = dis;
				minNN = _ordrho[j].second;
            }
        }
		_delta[_ordrho[i].second] = min;
		_nneigh[_ordrho[i].second] = minNN;
		
	
		_rhodelta.push_back({_ordrho[i].first * _delta[_ordrho[i].second],_ordrho[i].second});

		if(_rhodelta[i].first>secondMax_rhodelta){
			secondMax_rhodelta = _rhodelta[i].first;
			_delta[_ordrho[0].second] = _delta[_ordrho[i].second];
		}
        
    }
	_rhodelta[0].first = _ordrho[0].first * _delta[_ordrho[0].second]; 	
    
}


void dPeakClustering :: find_k_clustercenters(int k){
	sort(_rhodelta.rbegin(),_rhodelta.rend());
	_clusterCenters.resize(0); 
	for(int i = 0; i < k; i++){
		_clusterCenters.push_back(_rhodelta[i].second);
	}
}



void dPeakClustering :: clusterAssignment(){
	int numOfClusters = _clusterCenters.size();
	_clusters.resize(numOfClusters);
	_cId.resize(_totalPoints,-1);
	for(int cId = 0; cId < numOfClusters; cId++){
		int cc = _clusterCenters[cId];
		_cId[cc] = cId+1;
		_clusters[cId].push_back(cc);
	}
	for(int i = 0; i < _totalPoints; i++){
		int currPoint = _ordrho[i].second;
		if(_cId[currPoint] < 0){ 
			_cId[currPoint] = _cId[_nneigh[currPoint]];
			_clusters[_cId[currPoint]-1].push_back(currPoint);
		}	
	}
	
}




vector<double> dPeakClustering :: getDeltaVector() const{
    return this->_delta;
}
vector<int> dPeakClustering :: getNeighbourVector() const{
    return this->_nneigh;
}

vector<pair<double,int>> dPeakClustering :: getOrdRho(){
    return this->_ordrho;
}


