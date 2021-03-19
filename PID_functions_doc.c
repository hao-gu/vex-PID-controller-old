///tune/// YOU NEED TO TUNE THESE CONSTANTS FOR YOURSELF
//drive constants
double kp = 0.6; //
double ki = 0.0; //
double kd = 0.0; //

//turn costants
double tp = 0.75; //0.8
double ti = 0;
double td = 0.4;

//drift correction constant
double ktp = 1.3;
///tune///



//double proportion;
double totalError;
double rtotalError;
double integral;
double rintegral;
double integralActiveZone = 200;//adjust later
double integralPowerLimit = 50;//adjust later
double derivative;
double rderivative;
double leftPower;
double rightPower;
double terror; //turn error >:D
double gyroValue;
double gyroerror;
double turn_TotalError;
double tderivative;
double power;
double tlasterror;

int side;

task gyroTask()
{
	gyroValue = getGyroHeading(gyro);
}

void Drive(double target,double angle) {
	double error = target;
	double lastError = target;
	double rError = target;
	double rLastError = target;
	resetMotorEncoder(right);
	resetMotorEncoder(left);
	while(abs(error) > 2||abs(Rerror) > 2||abs(lasterror)>1){

		error = target - getMotorEncoder(left);
		rError = target - getMotorEncoder(right);
		//if(error == 0){
			//break;
		//}


		//left integral
		if(abs(error) > abs(2*target/3) && error != 0){
			totalError += error;
		}
		else totalError = 0.0;

		if(totalError > integralPowerLimit){
			totalError = integralPowerLimit;
		}
		if(totalError < -integralPowerLimit){
			totalError = -integralPowerLimit;
		}

		integral = ki * totalError;
		///////

		//right integral
		if(abs(rError) < abs(2*target/3) && rError != 0){
			rtotalError += rerror;
		}
		else rtotalError = 0.0;

		if(rtotalError > integralPowerLimit){
			rtotalError = integralPowerLimit;
		}
		if(rtotalError < -integralPowerLimit){
			rtotalError = -integralPowerLimit;
		}

		rintegral = ki * rtotalError;
		///////

		//derivative
		derivative = kd * (error - lastError);
		lastError = error;

		rderivative = kd * (rerror - rlastError);
		rlastError = rerror;

		if(error == 0){
			derivative = 0;
			rderivative = 0;
		}

		//terror = turn error lol
		terror = angle-getGyroHeadingFloat(gyro);

		if (terror > 180){
			terror = terror - 360;
		}
		else if (terror < -180){
			terror = terror + 360;
		}

		leftPower = 1 * (kp * error + derivative);
		rightPower = 1 * (kp * rError + rintegral + rderivative);
		if (leftPower > 100){
			leftPower=90;
		}
		if (rightpower > 100){
			rightpower=100;
		}
		setMotorSpeed(left,leftPower);
		setMotorSpeed(right,rightPower);
		wait1Msec(20);
	}
	StopMotor(left);
	StopMotor(right);
}

void constant_drive(double target, int speed, int angle, double k){
	float change;
	ktp = ktp;
	resetMotorEncoder(left);
	resetMotorEncoder(right);
	while(abs(target-getmotorencoder(left))>10)
	{
		terror = angle-getGyroHeadingFloat(gyro);

		if (terror > 180){
			terror = terror - 360;
		}
		else if (terror < -180){
			terror = terror + 360;
		}
		change = terror*ktp;
		setMotor(left,speed-change);
		setMotor(right,speed+change);
	}
	stopMotor(left);
	stopMotor(right);
	setMotorBrakeMode(left,motorBrake);
	setMotorBrakeMode(right,motorBrake);
	wait1Msec(300);
}

void sensor_drive(int times, int speed, int angle, double k){
	float change;
	ktp = ktp;
	resetMotorEncoder(left);
	resetMotorEncoder(right);
	for(int a=1;a<=times;a=a+1){
		while(getColorGrayscale(Color)>130)
		{
			terror = angle-getGyroHeadingFloat(gyro);

			if (terror > 180){
				terror = terror - 360;
			}
			else if (terror < -180){
				terror = terror + 360;
			}
			change = terror*ktp;
			setMotor(left,speed-change);
			setMotor(right,speed+change);
		}
		stopMotor(left);
		stopMotor(right);
		setMotorBrakeMode(left,motorBrake);
		setMotorBrakeMode(right,motorBrake);
		wait1Msec(300);
		}
	}

void Turn_to(double target){ //currently a PD loop.
	terror = target - getGyroHeading(gyro);
	tlasterror = terror;

	while(abs(terror) > 1.0||abs(terror-tlasterror)>0.2){
		terror = target - getGyroHeading(gyro);

		if (terror > 180){
			terror = terror - 360;
		}
		else if (terror < -180){
			terror = terror + 360;
		}

		tderivative = td * (terror - tlastError);
		tlastError = terror;

		if(terror == 0){
			tderivative = 0;
		}
		power = tp * terror + td * tderivative;

		setMotorSpeed(left,power*-1);
		setMotorSpeed(right,power*1);

		wait1Msec(30);

	}
	stopMotor(left);
	stopMotor(right);
}