
#pragma once

#include <iostream>
#include <string>
#include <utils/RefBase.h>
#include <scheduler/Timer.h>

#include <utils/Log.h>
#include <inttypes.h>
#include <chrono>

#include <csignal>
#include <unistd.h>
#include <sys/types.h>

using namespace android;
using namespace android::scheduler;


class MyTestRef:public RefBase{
    
  public:	
	MyTestRef(){
            MyTestRef(__FILE__,__FUNCTION__,__LINE__);	  
        }

	MyTestRef(std::string file,std::string fun,int line){
	    this->mFile = file;
	    this->mFun = fun;
	    this->mLine = line;
	    //printf("Construct MyTestRef Object.\n");
	    //ALOGE("Construct MyTestRef Object.\n");
	}

	~MyTestRef(){
            //printf("Destory MyTestRef Object. \n");
	    //ALOGE("Destory MyTestRef Object. \n");
	}

	void onFirstRef(){
	    //this->mFile = __FILE__;
	    //this->mFun =  __FUNCTION__;    
	    //this->mLine = __LINE__;
	    //std::cout << "file: " << this->mFile << " ,func:" << this->mFun <<", line: " << this->mLine << std::endl;
	    ALOGE("MyTestRef-begin-FILE: %s ,FUNC: %s ,LINE: %d",this->mFile.c_str(),this->mFun.c_str(),this->mLine);

	    mTimeStart = mTimeKeeper->now();
	    //constexpr std::chrono::nanoseconds kSnapToSameVsyncWithin = 3ms;
	    //constexpr std::chrono::nanoseconds time_gap = 3ms;
	    std::chrono::seconds sec(1200);
	    std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(sec);
	    nsecs_t target_time = mTimeKeeper->now() + ns.count();

	    //ALOGE("target_time = %" PRId64 "\n",target_time);

            mTimeKeeper->alarmAt(std::bind(&MyTestRef::timerCallback, this), target_time);
	}

	void onLastStrongRef(const void* id){
            mTimeKeeper->alarmCancel();
	    std::cout << "file: " << this->mFile << " ,func:"<< this->mFun <<", line: " << this->mLine << id << std::endl;
	    ALOGE("MyTestRef-  end-FILE: %s ,FUNC: %s ,LINE: %d",this->mFile.c_str(),this->mFun.c_str(),this->mLine);
	    float time_elapse = (mTimeKeeper->now() - mTimeStart)/1e9f;
	    if (time_elapse > 3.0){
                ALOGE("MyTestRef- alive %.2fs ",time_elapse);
	    }
	}


	void timerCallback(){
            if (!MyTestRef::s_blocked.exchange(true)) {
		//std::cout << "blocked here" << std::endl;
		ALOGE("MyTestRef-blocked-FILE: %s ,FUNC: %s ,LINE: %d",this->mFile.c_str(),this->mFun.c_str(),this->mLine);
		//ALOGE("blocked here");
		pid_t pid = getpid();
		ALOGE("send sigkill to %d",pid);
		//raise(SIGKILL);
		raise(SIGSEGV);
		ALOGE("after send sigkill to %d",pid);

	    }else{
	        while(true){
		    ALOGE("had-blocked-FILE: %s ,FUNC: %s ,LINE: %d",this->mFile.c_str(),this->mFun.c_str(),this->mLine);
		    sleep(1);
		    ALOGE("sleep and wait 1s");
		}
	    }
	}


  private:
	std::string mFile;
	std::string mFun;
	int mLine;
	nsecs_t mTimeStart;
	std::unique_ptr<TimeKeeper> const mTimeKeeper = std::make_unique<Timer>();
  public:
	static std::atomic_bool  s_blocked;
};

//std::atomic_bool MyTestRef::s_blocked(false);
