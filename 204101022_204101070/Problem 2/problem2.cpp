#include<bits/stdc++.h>
#include<random>
#include<time.h>

using namespace std;


/* ============CASE A Start===============================================================*/

void caseA(double lambda, double mu, double totalTimeUnits){

    ofstream answer;  //Creates a object of file to store the answer
    answer.open("CaseA.txt");  // Creates a file CaseA.txt if it does not exist else opens the existing file.
    double totalAT = 0;  //stores total arrival time.
    double totalCT1 = 0, totalCT2 = 0;  //stroes the total completion time of each officer.
    double totalWT1 = 0, totalWT2 = 0;  //Total waiting time for both the officers.
    double totalRT1 = 0, totalRT2 = 0;  //Total response time for both the officers.
    double waitTime = 0;  //Stores the waitime of each migrant.
    int count1 = 0, count2 = 0;  //Stores the number of migrants who will wait in the queue before getting checked.

    exponential_distribution<double> arrival_timing(lambda);  //arrival timings follow exponential distribution hence using this inbuilt function to generate this distribution using lambda.
    exponential_distribution<double> service_timing(mu);  //service timings follow exponential distribution hence using this inbuilt function to generate this distribution using mu.
    default_random_engine generator(time(0));  //generates a random number.
    
    double t = totalTimeUnits;  //Initiates t variable with total time units we observe the queue.
    vector<double> arrivaltime1,arrivaltime2;  //Two vectors which store the arrival timings of each migrant for each officer.
    
    double intTime = 0;  //Intermediate time which stores the service time or arrival interval as and when required.
    bool flag = 0;  //Flag used for adding migrants in the queue of both the officers.
    double finTime = 0;  //Contains the arrival time of the migrant by adding the arrival interval to the arriving time.
    
    //Used for storing the arrival time of both the queues
    while(t>0){
        intTime = arrival_timing(generator);  //Generates the arrival interval between 2 migrants
        finTime+=intTime;  //Calculates the arriving time using the arriving interval.
        //Now as we are given 0.5 probability hence we push each migrant one by one in towards each officer alternately
        if(flag == 0){
            arrivaltime1.push_back(finTime); 
            flag = 1;
        }
        else{
            arrivaltime2.push_back(finTime); 
            flag = 0;
        }
        
        totalAT +=  intTime;  //Calculates the total arrival time
        t-=intTime;  //Subtracts the arrival interval from the total time for which we observe the queue.
    }
    
    int size1 = arrivaltime1.size();  //Stores the number of migrants in the queue 1
    int i=0;

    double prevTime1 = arrivaltime1[0];  //Stores the completion time of the latest migrant by the first officer.
    
    //Run the loop until all the migrants of queue 1 are being served.
    while(size1--){
        intTime = service_timing(generator);  //Generates a service time according to the exponential distribution
        
        //If previous migrants completion is less than arrival time of current migrant then the officer has to wait.
        //hence we take the new completion time of the previous migrant as the arrival time of current migrant and now perform the operations.
        if(prevTime1 < arrivaltime1[i]){
            prevTime1 = arrivaltime1[i];
        }

        totalCT1 = intTime+prevTime1;  //Updating the completion time of the first officer.
        prevTime1 = totalCT1;  //Setting the previous completion time of officer 1 in prevTime1 variable.

        waitTime = (totalCT1 - arrivaltime1[i] - intTime);  //Calculating the wait time for this particular migrant
            
        //If the wait time is more than zero means that the migrant has waited for some time hence we will increase the counter by 1.
        if(waitTime > 0){
            count1++;
        }
        totalWT1 += waitTime;  //Adding the total waiting time
        totalRT1 += (totalCT1- arrivaltime1[i]);  //Calculating the total response time of the first officer.
        i++;
    }
    cout<<"Total Waiting Time for officer 1 : "<<totalWT1<<endl;
    answer<<"Total Waiting Time for officer 1 : "<<totalWT1<<endl;
    
    int size2 = arrivaltime2.size();  //Stores the number of migrants in the queue 2
    double prevTime2 = arrivaltime2[0]; //Stores the completion time of the latest migrant by the second officer.
    i = 0;

    //Run the loop until all the migrants of queue 2 are being served.
    while(size2--){
        intTime = service_timing(generator);  //Generates a service time according to the exponential distribution
        

        //If previous migrants completion is less than arrival time of current migrant then the officer has to wait.
        //hence we take the new completion time of the previous migrant as the arrival time of current migrant and now perform the operations.
        if(prevTime2 < arrivaltime2[i]){
            prevTime2 = arrivaltime2[i];
        }
        totalCT2 = intTime+prevTime2;  //Updating the completion time of the second officer.
        prevTime2 = totalCT2;  //Setting the previous completion time of officer 2 in prevTime2 variable.

        waitTime = (totalCT2 - arrivaltime2[i] - intTime);  //Calculating the wait time for this particular migrant
            
        //If the wait time is more than zero means that the migrant has waited for some time hence we will increase the counter by 1.
        if(waitTime > 0){
            count2++;
        }
        totalWT2 += waitTime;  //Adding the total waiting time
        totalRT2 += (totalCT2- arrivaltime2[i]);  //Calculating the total response time of the second officer.
        i++;
    }
    cout<<"Total Waiting Time for officer 2 : "<<totalWT2<<endl;
    answer<<"Total Waiting Time for officer 2 : "<<totalWT2<<endl;
    size1 = arrivaltime1.size();  //Re-initalizing the value of number of migrants in the first queue.

    cout<<endl<<"Case A"<<endl<<endl;
    cout<<"Values for Officer 1 : "<<endl;
    cout<<"========================================================================================="<<endl;
    cout<<"Average number of workers getting checked : "<<(double)size1/(double)totalCT1<<endl;
	cout<<"Average response time for workers in getting checked : "<<(double)totalRT1/(double)size1<<endl;
	cout<<"Average time for which a worker has to wait until getting checked : "<<(double)totalWT1/(double)size1<<endl;
	cout<<"Average number of workers waiting in the queue before each officer : "<<(double)count1/(double)size1<<endl;
    cout<<"========================================================================================="<<endl;
    cout<<endl<<endl;


    answer<<endl<<"Case A"<<endl<<endl;
    answer<<"Values for Officer 1 : "<<endl;
    answer<<"========================================================================================="<<endl;
    answer<<"Average number of workers getting checked : "<<(double)size1/(double)totalCT1<<endl;
    answer<<"Average response time for workers in getting checked : "<<(double)totalRT1/(double)size1<<endl;
    answer<<"Average time for which a worker has to wait until getting checked : "<<(double)totalWT1/(double)size1<<endl;
    answer<<"Average number of workers waiting in the queue before each officer : "<<(double)count1/(double)size1<<endl;
    answer<<"========================================================================================="<<endl;
    answer<<endl<<endl;

    size2 = arrivaltime2.size();  //Re-initalizing the value of number of migrants in the second queue.

    cout<<"Values for Officer 2 : "<<endl;
    cout<<"========================================================================================="<<endl;
    cout<<"Average number of workers getting checked : "<<(double)size2/(double)totalCT2<<endl;
	cout<<"Average response time for workers in getting checked: "<<(double)totalRT2/(double)size2<<endl;
	cout<<"Average time for which a worker has to wait until getting checked : "<<(double)totalWT2/(double)size2<<endl;
	cout<<"Average number of workers waiting in the queue before each officer : "<< (double)count2/(double)size2<<endl;
    cout<<"========================================================================================="<<endl;
    cout<<endl<<endl;

    answer<<"Values for Officer 2 : "<<endl;
    answer<<"========================================================================================="<<endl;
    answer<<"Average number of workers getting checked : "<<(double)size2/(double)totalCT2<<endl;
    answer<<"Average response time for workers in getting checked: "<<(double)totalRT2/(double)size2<<endl;
    answer<<"Average time for which a worker has to wait until getting checked : "<<(double)totalWT2/(double)size2<<endl;
    answer<<"Average number of workers waiting in the queue before each officer : "<< (double)count2/(double)size2<<endl;
    answer<<"========================================================================================="<<endl;
    answer<<endl<<endl;
}

/* ============CASE A End===============================================================*/



/* ============CASE B Start===============================================================*/
void caseB(double lambda, double mu, double totalTimeUnits){

    ofstream answer;
    answer.open("CaseB.txt");
    double totalAT = 0;  //stores total arrival time.
    double totalCT1 = 0, totalCT2 = 0;  //stroes the total completion time of each officer.
    double totalWT = 0;  //Total waiting time for both the officers.
    double totalRT = 0;  //Total response time for both the officers.

    exponential_distribution<double> arrival_timing(lambda);  //arrival timings follow exponential distribution hence using this inbuilt function to generate this distribution using lambda.
    exponential_distribution<double> service_timing(mu);  //service timings follow exponential distribution hence using this inbuilt function to generate this distribution using mu.
    default_random_engine generator(time(0));  //generates a random number.
    
    double t = totalTimeUnits;  //Initiates t variable with total time units we observe the queue.
    vector<double> arrivaltime;  //vector which store the arrival timings of each migrant.
    
    double intTime = 0;  //Intermediate time which stores the service time or arrival interval as and when required.
    bool flag = 0;  //Flag used for adding migrants in the queue of both the officers.
    double finTime = 0;  //Contains the arrival time of the migrant by adding the arrival interval to the arriving time.
    
    //Used for storing the arrival time of both the queues
    while(t>0){
        intTime = arrival_timing(generator);  //Generates the arrival interval between 2 migrants
        finTime+=intTime;  //Calculates the arriving time using the arriving interval.
        arrivaltime.push_back(finTime);  //Stores all the arrival time of migrants in the vector
        totalAT +=  intTime;  //Calculates the total arrival time
        t-=intTime;  //Subtracts the arrival interval from the total time for which we observe the queue.
    }
    
    int size = arrivaltime.size();  //Stores the number of migrants in the queue
    int i=0;
    double prevTime1 = arrivaltime[0];    //Stores the completion time of the latest migrant by the first officer.
    double prevTime2 = arrivaltime[1];  //Stores the completion time of the latest migrant by the second officer.
    double waitTime = 0;  //Stores the waitime of each migrant.
    int count = 0;  //Stores the number of migrants who will wait in the queue before getting checked.

    //Run the loop until all the migrants of queue 1 are being served.
    while(size--){
        intTime = service_timing(generator);  //Generates a service time according to the exponential distribution

        //Initiates prevTime1 and prevTime2 with the completion time of first two migrants
        if(prevTime1 == arrivaltime[0]){
            totalCT1 = intTime+prevTime1;  //Update the total completion time of first officer by service time and arrival time of first migrant
            prevTime1 = totalCT1;  //Update previous completion time
            i++;
            //cout<<"Inside prevTime1 "<<endl;
            continue;
        }
        else if(prevTime2 == arrivaltime[1]){
            totalCT2 = intTime+prevTime2;  //Update the total completion time of second officer by service time and arrival time of first migrant
            prevTime2 = totalCT2;  //Update previous completion time
            i++;
            //cout<<"Inside prevTime2 "<<endl;
            continue;
        }
        
        //cout<<"Outside prevTime1 & prevTime2 "<<endl;

        
        //If completion time of the migrants in the queue of officer 1 is less than completion time of officer 2 then send the new migrant who has just arrived to officer 1.
        //else send that migrant to officer 2.
        if(prevTime1 < prevTime2){

            //If previous migrants completion is less than arrival time of current migrant then the officer has to wait.
            //hence we take the new completion time of the previous migrant as the arrival time of current migrant and now perform the operations.
            if(prevTime1 < arrivaltime[i]){
                prevTime1 = arrivaltime[i];  //Update the previous completion time.
            }

            totalCT1 = intTime+prevTime1;  //Updating the completion time of the first officer.
            prevTime1 = totalCT1;  //Setting the previous completion time of officer 1 in prevTime1 variable.
            waitTime = (totalCT1 - arrivaltime[i] - intTime);  //Calculating the wait time for this particular migrant
            
            //If the wait time is more than zero means that the migrant has waited for some time hence we will increase the counter by 1.
            if(waitTime > 0){
                count++;
            }
            totalWT += waitTime;  //Adding the total waiting time
            totalRT += (totalCT1 - arrivaltime[i]);  //Calculating the total response time.
        }
        else{

            //If previous migrants completion is less than arrival time of current migrant then the officer has to wait.
            //hence we take the new completion time of the previous migrant as the arrival time of current migrant and now perform the operations.
            if(prevTime2 < arrivaltime[i]){
                prevTime2 = arrivaltime[i];  //Update the previous completion time.
            }

            totalCT2 = intTime+prevTime2;  //Updating the completion time of the second officer.
            prevTime2 = totalCT2;  //Setting the previous completion time of officer 2 in prevTime2 variable.
            waitTime = (totalCT2 - arrivaltime[i] - intTime);  //Calculating the wait time for this particular migrant
            //If the wait time is more than zero means that the migrant has waited for some time hence we will increase the counter by 1.
            if(waitTime > 0){
                count++;
            }
            totalWT += waitTime;  //Adding the total waiting time
            totalRT += (totalCT2 - arrivaltime[i]);  //Calculating the total response time.
        }
        
        i++;
    }
    cout<<"Total Waiting Time : "<<totalWT<<endl;
    answer<<"Total Waiting Time : "<<totalWT<<endl;

    double totalCT = max(totalCT1,totalCT2);

    size = arrivaltime.size();   //Re-initalizing the value of number of migrants in the queue.

    cout<<endl<<"Case B"<<endl<<endl;
    cout<<"Values for Officer : "<<endl;
    cout<<"========================================================================================="<<endl;
    cout<<"Average number of workers getting checked : "<<(double)size/(double)totalCT<<endl;
    cout<<"Average response time for workers in getting checked : "<<(double)totalRT/(double)size<<endl;
    cout<<"Average time for which a worker has to wait until getting checked : "<<(double)totalWT/(double)size<<endl;
    cout<<"Average number of workers waiting in the queue before each officer : "<<(double)count/(double)size<<endl;
    cout<<"========================================================================================="<<endl;
    cout<<endl<<endl;
    cout<<endl<<endl;

    answer<<endl<<"Case B"<<endl<<endl;
    answer<<"Values for Officer : "<<endl;
    answer<<"========================================================================================="<<endl;
    answer<<"Average number of workers getting checked : "<<(double)size/(double)totalCT<<endl;
    answer<<"Average response time for workers in getting checked : "<<(double)totalRT/(double)size<<endl;
    answer<<"Average time for which a worker has to wait until getting checked : "<<(double)totalWT/(double)size<<endl;
    answer<<"Average number of workers waiting in the queue before each officer : "<< (double)count/(double)size<<endl;
    answer<<"========================================================================================="<<endl;
    answer<<endl<<endl;
    answer<<endl<<endl;
    
    answer.close();
}

/* ============CASE B End===============================================================*/


/* ============CASE C Start===============================================================*/

void caseC(double lambda, double mu, double totalTimeUnits){   

    ofstream answer;
    answer.open("CaseC.txt");

    double totalAT = 0;  //stores total arrival time.
    double totalCT1 = 0, totalCT2 = 0;  //stroes the total completion time of each migrant.
    double totalWT1 = 0, totalWT2 = 0;  //Total waiting time for both the officers.
    double totalRT1 = 0, totalRT2 = 0;  //Total response time for both the officers.

    exponential_distribution<double> arrival_timing(lambda);  //arrival timings follow exponential distribution hence using this inbuilt function to generate this distribution using lambda.
    exponential_distribution<double> service_timing(mu);  //service timings follow exponential distribution hence using this inbuilt function to generate this distribution using mu.
    default_random_engine generator(time(0));  //generates a random number.
    
    double t = 0;  //Initiates t variable with total time units we observe the queue.
    queue<pair<double,double> > q1,q2;

    //vector<double> arrivaltime1,arrivaltime2;  //Two vectors which store the arrival timings of each migrant for each officer.
    
    double intTime = 0;  //Intermediate time which stores the service time or arrival interval as and when required.
    bool flag = 0;  //Flag used for adding migrants in the queue of both the officers.
    double finTime = 0;  //Contains the arrival time of the migrant by adding the arrival interval to the arriving time.
    double servTime;
    double prevTime1 = 0, prevTime2 = 0;
    int count1 = 0, count2 = 0;
    double waitTime;
    int size1 = 0, size2 = 0;

    while(t<=totalTimeUnits){
        intTime = arrival_timing(generator);
        finTime+=intTime;
        if(flag == 0){
        	if(q1.size()<=5){
        		q1.push({finTime,0}); 
        		servTime = service_timing(generator);  //Generates a service time according to the exponential distribution
        		if(prevTime1 < finTime){
        			prevTime1 = finTime;
        		}
        		totalCT1 = prevTime1+servTime;
        		prevTime1 = totalCT1;
        		waitTime = (totalCT1 - finTime - servTime);  //Calculating the wait time for this particular migrant
            	//If the wait time is more than zero means that the migrant has waited for some time hence we will increase the counter by 1.
            	if(waitTime > 0){
                	count1++;
            	}
            	totalWT1 += waitTime;  //Adding the total waiting time
            	totalRT1 += (totalCT1 - finTime);  //Calculating the total response time.
        		q1.back().second = totalCT1;
        		size1++;
            	flag = 1;
        	}
        	else{
        		t+=intTime;
        		continue;
        	}
            
        }
        else{
        	if(q2.size() <= 5){
        		q2.push({finTime,0});
        		servTime = service_timing(generator);  //Generates a service time according to the exponential distribution
        		if(prevTime2 < finTime){
        			prevTime2 = finTime;
        		}
        		totalCT2 = prevTime2+servTime;
        		prevTime2 = totalCT2;
        		waitTime = (totalCT2 - finTime - servTime);  //Calculating the wait time for this particular migrant
            	//If the wait time is more than zero means that the migrant has waited for some time hence we will increase the counter by 1.
            	if(waitTime > 0){
                	count2++;
            	}
            	totalWT2 += waitTime;  //Adding the total waiting time
            	totalRT2 += (totalCT2 - finTime);  //Calculating the total response time.
        		q2.back().second = totalCT2; 
            	size2++;
            	flag = 0;
        	}
        	else{
        		t+=intTime;
        		continue;
        	}
        }
        if(q1.front().second < t){
        	q1.pop();
        }
        if(q2.front().second < t){
        	q2.pop();
        }
        totalAT +=  intTime;
        t+=intTime;
    }
    cout<<"Total Waiting Time : "<<totalWT1<<endl;
    answer<<"Total Waiting Time : "<<totalWT1<<endl;
    cout<<"Total Waiting Time : "<<totalWT2<<endl;
    answer<<"Total Waiting Time : "<<totalWT2<<endl;

    //double totalCT = max(totalCT1, totalCT2);

    cout<<endl<<"Case C"<<endl<<endl;
    cout<<"Values for Officer 1: "<<endl;
    cout<<"========================================================================================="<<endl;
    cout<<"Average number of workers getting checked : "<<(double)size1/(double)totalCT1<<endl;
    cout<<"Average response time for workers in getting checked : "<<(double)totalRT1/(double)size1<<endl;
    cout<<"Average time for which a worker has to wait until getting checked : "<<(double)totalWT1/(double)size1<<endl;
    cout<<"Average number of workers waiting in the queue before each officer : "<<(double)count1/(double)size1<<endl;
    cout<<"========================================================================================="<<endl;
    cout<<endl<<endl;
    cout<<endl<<endl;

    answer<<endl<<"Case C"<<endl<<endl;
    answer<<"Values for Officer 1: "<<endl;
    answer<<"========================================================================================="<<endl;
    answer<<"Average number of workers getting checked : "<<(double)size1/(double)totalCT1<<endl;
    answer<<"Average response time for workers in getting checked : "<<(double)totalRT1/(double)size1<<endl;
    answer<<"Average time for which a worker has to wait until getting checked : "<<(double)totalWT1/(double)size1<<endl;
    answer<<"Average number of workers waiting in the queue before each officer : "<< (double)count1/(double)size1<<endl;
    answer<<"========================================================================================="<<endl;
    answer<<endl<<endl;
    answer<<endl<<endl;

    cout<<"Values for Officer 2: "<<endl;
    cout<<"========================================================================================="<<endl;
    cout<<"Average number of workers getting checked : "<<(double)size2/(double)totalCT2<<endl;
    cout<<"Average response time for workers in getting checked : "<<(double)totalRT2/(double)size2<<endl;
    cout<<"Average time for which a worker has to wait until getting checked : "<<(double)totalWT2/(double)size2<<endl;
    cout<<"Average number of workers waiting in the queue before each officer : "<<(double)count2/(double)size2<<endl;
    cout<<"========================================================================================="<<endl;
    cout<<endl<<endl;
    cout<<endl<<endl;

    answer<<endl<<"Case C"<<endl<<endl;
    answer<<"Values for Officer 2: "<<endl;
    answer<<"========================================================================================="<<endl;
    answer<<"Average number of workers getting checked : "<<(double)size2/(double)totalCT2<<endl;
    answer<<"Average response time for workers in getting checked : "<<(double)totalRT2/(double)size2<<endl;
    answer<<"Average time for which a worker has to wait until getting checked : "<<(double)totalWT2/(double)size2<<endl;
    answer<<"Average number of workers waiting in the queue before each officer : "<< (double)count2/(double)size2<<endl;
    answer<<"========================================================================================="<<endl;
    answer<<endl<<endl;
    answer<<endl<<endl;

    answer.close();
}

/* ============CASE C End===============================================================*/

int main(){
	
    double lambda,mu,totalTimeUnits;  //These variables are used to input lambda, mu and total time which the queue is being observed.
    
    cout<<"Enter the value of mean arrival rate i.e. lambda = ";
    cin>>lambda;  //Taking arrival rate (lambda) input from the user.
    
    cout<<endl<<"Enter the value of mean service rate i.e. mu = ";
    cin>>mu;  //Taking service rate (mu) input from the user
    
    cout<<endl<<"Enter the total time units you want to observe the queue = ";
    cin>>totalTimeUnits;  //Taking the time in units we want to observe the queue.
    cout<<endl;
    
    bool flag = 0;
    int casenumber;  //Stores the case number which the user inputs to enter a particular case.
    

    //Coninues the loop until the user wants to exit.
    while(1){
    	
        cout<<"Which case do you want to run? "<<endl;
        cout<<"Press 1 for case A"<<endl;
        cout<<"Press 2 for case B"<<endl;
        cout<<"Press 3 for case C"<<endl;
        cout<<"Press 4 for EXIT"<<endl;
        cout<<"Enter your option : ";
        cin>>casenumber;  //Takes input the case which the user wants to run.
        
        //Switches to the case where user wants to go
        switch(casenumber){
    
            case 1 : caseA(lambda,mu,totalTimeUnits);
                    break;
    
            case 2  : caseB(lambda,mu,totalTimeUnits);
                    break;
    
            case 3 : caseC(lambda,mu,totalTimeUnits);
                    break;
    
            case 4 : flag = 1;  //Sets the flag to 1 so that the user could exit the while loop.
                    break;
    
            default : cout<<"You have entered the wrong case number. Please re-enter."<<endl<<endl;
            		break;
        }

        //If flag is set to 1 means the user wants to exit then set it to zero and break out of the loop.
        if(flag == 1){
            flag = 0;
            break;
        }
    }
    return 0;
}
