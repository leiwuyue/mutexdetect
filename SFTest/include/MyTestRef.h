
#pragma once

#include <iostream>
#include <string>
#include <utils/RefBase.h>
#include <scheduler/Timer.h>

using namespace android;
using namespace android::scheduler;


class MyTestRef:public RefBase{
    
  public:	
	MyTestRef(){
	    printf("Construct MyTestRef Object.\n");
	}

	~MyTestRef(){
        printf("Destory MyTestRef Object. \n");
	}

	void onFirstRef(){
	    this->mFile = __FILE__;
	    this->mFun =  __FUNCTION__;    
	    this->mLine = __LINE__;
	    std::cout << "file: " << this->mFile << " ,func:" << this->mFun <<", line: " << this->mLine << std::endl;
            mTimeKeeper->alarmAt(std::bind(&MyTestRef::timerCallback, this), 10 * 000000000);
	}

	void onLastStrongRef(const void* id){
            mTimeKeeper->alarmCancel();
	    std::cout << "file: " << this->mFile << " ,func:"<< this->mFun <<", line: " << this->mLine << std::endl;
	}


	void timerCallback(){
        if (!MyTestRef::s_blocked.exchange(true)) {
		std::cout << "blocked here" << std::endl;
	    }else{
	        while(true) sleep(1);
	    }
	}


  private:
	std::string mFile;
	std::string mFun;
	int mLine;
	std::unique_ptr<TimeKeeper> const mTimeKeeper = std::make_unique<Timer>();
  public:
	static std::atomic_bool  s_blocked;
};

std::atomic_bool MyTestRef::s_blocked(false);