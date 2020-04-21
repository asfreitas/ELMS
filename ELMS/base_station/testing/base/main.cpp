#include <iostream>
#include "vehicle.h"

using std::cout;
using std::endl;

int main(){                                                                                                                                                                                                                                                                                                                                                                             

    	cout << "HELLO WORLD" << endl;

 //   Vehicle v1;

   	Vehicle v1(1, 123456, 1234.5246, 9876.5412, 415.235, 452.336);
    	Vehicle v2(2, 6554, 1425.12, 875.23, 123.1, 652.45);

	int unitNum = v1.getUnit();

    	int timeNum = v1.getTime();

	printf("Unit is %d\n", unitNum);
	printf("Time is %d\n", timeNum);

	if(v1.checkVehicleVector(v2.getUnit()) == false){
		cout << "FALSE" << endl;
		v1.addVehicleVector(v2, 0);
		cout << "ADDED TO VECTOR" << endl;
	}else{
		cout << "TRUE" << endl;
	}

	if(v1.checkVehicleVector(v2.getUnit()) == false){
		cout << "FALSE" << endl;
		v1.addVehicleVector(v2, 1);
	}else{
		cout << "TRUE" << endl;
	}

	

    	return 0;

}
