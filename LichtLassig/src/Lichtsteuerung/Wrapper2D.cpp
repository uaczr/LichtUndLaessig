/*
 * Wrapper2D.cpp
 *
 *  Created on: 31.05.2015
 *      Author: christoph
 */

#include "Wrapper2D.h"

Wrapper2D::Wrapper2D(string path) {
	// TODO Auto-generated constructor stub

}

Wrapper2D::~Wrapper2D() {
	// TODO Auto-generated destructor stub
}

void Wrapper2D::initFromCSV(string path){
	vector<int> vstrip;
	vector<int> vled;
	vector<double> vx;
	vector<double> vy;
	int strip, led, max_length, r, g, b;
	double x, y, z;

	//lese CSV ein
	io::CSVReader<5, io::trim_chars<' ', '\t'>, io::no_quote_escape<','>,io::throw_on_overflow, io::no_comment> in(path);
	in.read_header(io::ignore_missing_column, "max_length", "m", "n", "width", "length");
	in.read_row(max_length, m, n, width, length);
	in.read_header(io::ignore_missing_column, "strip", "led", "x", "y","empty");
	while(in.read_row(strip, led, x, y, z)){
		vstrip.push_back(strip);
		vled.push_back(led);
		vx.push_back(x);
		vy.push_back(y);
	}

	//speichere Pixelvector


	//bereite Raster vor
	raster = new Wrapper2Dfield*[m];
	raster[0] = new Wrapper2Dfield[n*m];
	for(int i = 0; i < m; i++){
		raster[i] = raster[0] + i*n;
	}

	double delta_x = width/(m);
	double delta_y = length/(n);

	for(int i = 0; i < m; i ++){
		for(int j = 0; j < n; j++){
			raster[i][j].x0 = m*delta_x;
			raster[i][j].x1 = (m + 1)*delta_x;
			raster[i][j].y0 = n*delta_y;
			raster[i][j].y1 = (n+1)*delta_y;
		}
	}



}
