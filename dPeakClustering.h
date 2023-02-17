#ifndef DPEAK_H
#define DPEAK_H
#pragma once
#include "data.h"
#include "distance.h"
#include "data.cpp"
#include "distance.cpp"
#include "localDensity.h"
#include "localDensity.cpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>

class dPeakClustering
{
    public:
        
    dPeakClustering(const vector<vector<double>> &, const double &);                                                            
    virtual ~dPeakClustering();

    void computeRelativeDistance();
    void computeLocalDensity();
	void find_k_clustercenters(int);
	void clusterAssignment();
    vector<double> getDeltaVector() const;
    vector<int> getNeighbourVector() const;
    vector<pair<double,int>>getOrdRho();
   // private: 
    
	const vector<vector<double>> & _distMatrix;
	double _Dc;
	int _totalPoints;
	double _maxDistance;
    vector<double> _delta;
	vector<double> _rho;
	vector<pair<double,int>>_rhodelta;
    vector<int> _nneigh;
    vector<pair<double,int>> _ordrho;
	vector<int> _clusterCenters;
	vector<vector<int>> _clusters;
	vector<int> _cId;
	vector<int> _haloId;
	vector<int> halo_points_boundary;
	vector<vector<int>> _halo;
};

#endif 
