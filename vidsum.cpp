//
//  main.cpp
//  vidsum
//
//  Created by Evan Binder and Nick Dodd on 4/24/19.
//  Copyright © 2019 Evan Binder and Nick Dodd. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include "CoreFoundation/CoreFoundation.h"


using namespace std;


int main(int argc, const char * argv[]) {
    // insert code here...
    
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    
    /************************************/
    /***********Getting Input************/
    /************************************/
    
    chdir(path);
    std::cout << "Current Path: " << path << std::endl;
    
    for (int i = 0; i < argc; i++){
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    cout<<"argc = "<<argc<<endl;
    string inpath = "input.mp4";
    string outpath = "output.mp4";
    if(argc <= 1 || strncmp(argv[1], "help", 5)==0){
        cout<<"Example usage:\n  ./vidsum INPUTVIDEO\n  ./vidsum INPUTVIDEO OUTPUTVIDEO\n  ./vidsum INPUTVIDEO OUTPUTVIDEO SPEED1 SPEED2\n\nDefault values:\n  OUTPUTVIDEO = out.mp4\n  SPEED1 = 1.0\n  SPEED2 = 2.0\n\nMore info available at github.com/evnb/vwrt\n"<<endl;
        return 0;
    }
    if(argc > 1){
        inpath = argv[1];
    }
    if(argc > 2){
        outpath = argv[2];
    }
    cout<<"Detecting Silence"<<endl;
    string volcall = "ffmpeg -i \"" + inpath + "\" -af silencedetect=noise=-30dB:d=0.5 -f null - 2> " + inpath + "vol.txt";
    system(volcall.c_str());
    
    /************************************/
    /***********File Analysis************/
    /************************************/
    cout<<"Starting File Analysis"<<endl;
    
    string s;
    string sTotal;
    string sNew;
    
    ifstream in;
    in.open(inpath + "vol.txt");
    
    //loads entire document into sTotal
    while(!in.eof()) {
        getline(in, s);
        sTotal += s + "\n";
        
    }
    
    size_t pos = sTotal.find("silence_start"); //find location of first time
    sNew = sTotal.erase(0,pos); //delete everything prior to location found
    
    cout << "SILENCE TIME EXTRACTION" << endl;
    //SILENCE TIME EXTRACTION
    string term3 = "silence_duration";
    int bpos3 = 0;
    
    long double secsum = 0;
    
    while ((bpos3 = sNew.find(term3,bpos3)) != -1)
    {
        int posStart = 18 + bpos3;
        string timeDur = sNew.substr (posStart,7);
        
        float timeDr = stof (timeDur);
        secsum += timeDr;
        cout<< timeDr << "\t" << secsum << endl;
        bpos3 += term3.length();
    }
    cout<<secsum<< "\ts, or" << endl;
    cout<<secsum/60<< "\tm\t" << fmod(secsum,60) << "\ts" <<endl;
    
    
    cout << "START TIME EXTRACTION" << endl;
    //START TIME EXTRACTION
    vector<double> startArr;
    vector<double> endArr;
    
    string term = "silence_start";
    int bpos = 0;
    
    while ((bpos = sNew.find(term,bpos)) != -1)
    {
        int posStart = 15 + bpos;
        string timeStart = sNew.substr (posStart,7);
        
        float timeSrt = stof (timeStart);
        startArr.push_back(timeSrt);
        cout<< (timeSrt / 2) <<endl;
        bpos += term.length();
    }
    
    //    for (int i=0; i<startArr.size(); i++)
    //        cout << startArr[i] << endl;
    //
    //    cout << endl;
    
    cout << "END TIME EXTRACTION" << endl;
    //END TIME EXTRACTION
    string term2 = "silence_end";
    int bpos2 = 0;
    
    while ((bpos2 = sNew.find(term2,bpos2)) != -1)
    {
        int posStart = 13 + bpos2;
        string timeEnd = sNew.substr (posStart,7);
        
        float timeNd = stof (timeEnd);
        endArr.push_back(timeNd);
        cout<< (timeNd / 2) <<endl;
        bpos2 += term2.length();
    }
    
    
    for (int i=0; i<startArr.size(); i++)
        cout << startArr[i] << endl;
    
    cout << endl;
    
    for (int i=0; i<endArr.size(); i++)
        cout << endArr[i] << "\n";
    
    cout << endl;
    
    vector<double> totalArr(startArr);
    
    totalArr.insert( totalArr.end(), endArr.begin(), endArr.end() );
    sort(totalArr.begin(), totalArr.end());
    
    //hopefully removing all instances of duplicate times correctly:
    double currentTime;
    for (int i=0; i<totalArr.size() - 1; i++){
        currentTime = totalArr[i];
        if(totalArr[i+1]==currentTime){
            totalArr.erase(totalArr.begin()+i);
            totalArr.erase(totalArr.begin()+i);
            i--;
        }
    }
    //totalArr.erase( unique( totalArr.begin(), totalArr.end() ), totalArr.end() ); //removes duplicate times //fixes the 3:29 error
    
    for (int i=0; i<startArr.size(); i++)
        cout << startArr[i] << endl;
    
    cout << endl;
    

    
    for (int i=0; i<totalArr.size(); i++)
        cout << totalArr[i] << endl;
    
    cout << endl;
    
    in.close();
    

    //cout<<cmd<<endl;
    /************************************/
    /***********Video Editing************/
    /************************************/
    cout<<"Starting Video Editing"<<endl;
    
//    ffmpeg -i Soon.mp4 -filter_complex "[0:v]trim=0:2,setpts=PTS-STARTPTS[v1]; [0:v]trim=2:5,setpts=0.5*(PTS-STARTPTS)[v2]; [0:v]trim=5,setpts=PTS-STARTPTS[v3]; [0:a]atrim=0:2,asetpts=PTS-STARTPTS[a1]; [0:a]atrim=2:5,asetpts=PTS-STARTPTS,atempo=2[a2]; [0:a]atrim=5,asetpts=PTS-STARTPTS[a3]; [v1][a1][v2][a2][v3][a3]concat=n=3:v=1:a=1" -preset superfast -profile:v baseline output.mp4
    
    string cmd = "";
    string acmd = "";
    
    cmd+= "ffmpeg -loglevel verbose -i " + inpath + " -filter_complex \"";
    //cmd+= "[0:v]trim=0:2,setpts=PTS-STARTPTS[v1]; "
    
    //cout<<cmd<<endl;
    
    //vector<double> times{2,17}; //starts with next timecode after 0, ends with last timecode before the end of the video
    double first_speed = 1.0;
    double second_speed = 2.0;
    
    if(argc > 4){
        first_speed = stod(argv[3]);
        second_speed = stod(argv[4]);
    }
    
    vector<double> times(totalArr);
    
    int numTimes = times.size();
    
    cmd= cmd + "[0:v]trim=0:" + to_string(times[0]) + ",setpts=" + to_string(first_speed) + "*(PTS-STARTPTS)[v0]; ";
    acmd= acmd + "[0:a]atrim=0:" + to_string(times[0]) + ",asetpts=PTS-STARTPTS,atempo=" + to_string(first_speed) + "[a0]; ";
    
    //cmd+= "[0:v]trim=0:" + "2" + ",setpts=PTS-STARTPTS[v1]; ";
    double mySpeed;
    int i;
    for(i = 0; i < numTimes - 1; i++){
        mySpeed = i%2==1? first_speed : second_speed;
        cmd= cmd + "[0:v]trim=" + to_string(times[0 + i]) + ":" + to_string(times[1 + i]) + ",setpts=" + to_string(1 / mySpeed) + "*(PTS-STARTPTS)[v" + to_string(i+1) + "]; ";
        acmd= acmd + "[0:a]atrim=" + to_string(times[0 + i]) + ":" + to_string(times[1 + i]) + ",asetpts=PTS-STARTPTS,atempo=" + to_string(mySpeed) + "[a" + to_string(i+1) + "]; ";
    }
    cmd = cmd + "[0:v]trim=" + to_string(times[i]) + ",setpts=" + to_string(1/(i%2==1? first_speed : second_speed)) + "*(PTS-STARTPTS)[v" + to_string(i + 1) + "]; ";
    acmd= acmd + "[0:a]atrim=" + to_string(times[i]) + ",asetpts=PTS-STARTPTS,atempo=" + to_string(i%2==1? first_speed : second_speed) + "[a" + to_string(i + 1) + "]; ";
    
    //cout<<cmd<<endl;
    //cout<<acmd<<endl;
    
    string cnc = "";
    for(int j = 0; j < i + 2; j++){
        cnc = cnc + "[v" + to_string(j) + "][a" + to_string(j) + "]";
    }
    cnc = cnc + "concat=n=" + to_string(i + 2) + ":v=1:a=1";
    //cout<<cnc<<endl;
    
//    cout<<cnc<<endl;
//    cout<<acmd<<endl;
    
    cmd = cmd + acmd + cnc + "\" -preset superfast -profile:v baseline " + outpath /*+ " 2> " + outpath + "_log.txt"*/;
    
    

    system(cmd.c_str());
    cout << cmd.length() << endl;
    //cout << cmd << endl;
    
    if(false){
        //Time-Stamp Remapping: (eg for subtitles)
        double in_time = 0;
        vector<double> speeds;
        for(int k = 0; k < numTimes + 1; k++){
            speeds.push_back(i%2==0? first_speed : second_speed);
        }
        vector<double> cumu_out_times;
        cumu_out_times.push_back(times[0]);
        cout<<cumu_out_times[0]<<endl;
        for(int w = 1; w < numTimes; w++){
            cumu_out_times.push_back(cumu_out_times[w-1]+(1/speeds[w-1])*(times[w]-times[w-1]));
            cout<<cumu_out_times[w]<<endl;
        }
        int q;
        for(q = 0; q < numTimes; q++){
            if(in_time<times[q])
                break;
        }
        double out_time = cumu_out_times[q] + speeds[q]*(in_time-times[q]);
        
    }
    
    
    
    system("echo $PATH");
    //system("/usr/local/bin/ffmpeg");
    std::cout << "Hello, World!\n";
    return 0;
}
