// -*- C++ -*-
/* 
    \file gnuplot.h
    simple C++ interface to gnuplot
*/

#ifndef GNUPLOT_H
#define GNUPLOT_H

#include <cstdio>
#include <map>
#include <utility>
#include <string>
#include <sstream>
#include <list>

#define GNUPLOT_ONLY_INCLUDES
// these includes define FILE* OpenGnuplot(void) and void CloseGnuplot(FILE*) 
#ifdef _WIN32
#include "gnuplot_win32.h"
#else
#include "gnuplot_unix.h"
#endif 
#undef GNUPLOT_ONLY_INCLUDES

/** \defgroup baseclasses Base classes
*/
/// \ingroup baseclasses

/**
   Open a Gnuplot window and always show last n values of arbitrary defined channels.
   T can be std::string or int or anything else supports the << of streams.
   (It is used as name for a channel.)
*/


template<class T> class Gnuplot {

/// these includes define FILE* OpenGnuplot(void) and void CloseGnuplot(FILE*) 
#ifdef _WIN32
#include "gnuplot_win32.h"
#else
#include "gnuplot_unix.h"
#endif 
public: 
    typedef std::list<T> nameslist;
    /** which data type is our template */
    typedef T channel_name_type;

protected:
    /** data buffer for one channel, organized as ring buffer */
    class Dataset{
	double *buffer;
	int buffersize;
	int current;
    public:    
	std::string title;
	std::string style;
	bool show;
	/** contruct a data buffer with (gnuplot) title and style. */
	Dataset(const std::string& title="",const std::string& style="",int buffersize=256)
	    :buffersize(buffersize),title(title),style(style){
	    buffer=new double[buffersize];
	    for(int i=0;i<buffersize;++i)
		buffer[i]=0.0;
	    current=0;
	    show=true;
	};
	~Dataset(){
	    delete []buffer;
	};
	/** put new data in (ring) buffer */
	void putData(double v){
	    buffer[current++]=v;
	    current=current%buffersize;
	};
	/** print buffer content to file pointer, usually a pipe */
	void plot(FILE* f, int start=0, int end=0){
	    int _start=current+start;
	    int _end=current+((end<=0)?buffersize:end);
	    for(int i=_start;i<_end;++i)
		fprintf(f,"%f\n",buffer[i%buffersize]);
	};
	/** get buffer content */
	double getData(int i){
	    return buffer[(i+current)%buffersize];
	};

    };

    FILE* pipe;
    /** data type of container storing   channel->data  relation */
    typedef std::map<T, Dataset*> dataset_map;
    dataset_map datasets;
    nameslist namesets;    
    int buffersize;
    int start;
    int end;
public:
    
    Gnuplot(int buffersize=256):buffersize(buffersize),start(0),end(buffersize){
	pipe=OpenGnuplot();
    };
    ~Gnuplot(){
	for(typename dataset_map::iterator i=datasets.begin();i!=datasets.end();++i){
	    delete i->second;
	}
	if(pipe)CloseGnuplot(pipe);
    };
    /** send arbitrary command to gnuplot.
	like "set zeroaxis" or other stuff */
    void command(const std::string& cmd){
	fprintf(pipe,"%s\n",cmd.data());
	fflush(pipe);
    };
    /** add new channel with name. 
	currently uses name as title if not given.  */
    void addChannel(const T& name, const std::string& title="", const std::string& style="lines"){
	if(title != "")
	    datasets.insert(std::make_pair(name,new Dataset(title,style,buffersize)));
	else {
	    std::ostringstream str;
	    str << name;
	    datasets.insert(std::make_pair(name,new Dataset(str.str(),style,buffersize)));
	}
        namesets.push_back(name);
    };
    /** add new data value to buffer for channel.
        if channel not exists nothing happens.
	So you can select channels in your program by commenting out the addChannel() call */
    void putData(const T& channel, double data){
	typename dataset_map::iterator i = datasets.find(channel);
	if(i!=datasets.end())
	    i->second->putData(data);
    }; 

    /** returns channel names */
    const nameslist& getNames(){
    	return namesets;
    };
    
    /** get title  */
    std::string getTitle(const T& channel){
	typename dataset_map::iterator i = datasets.find(channel);
	if(i!=datasets.end())
	    return i->second->title;
	return "";
    }; 
    
    /** switch showing of this channel on or off */
    void show(const T& channel, bool on=true){
	typename dataset_map::iterator i = datasets.find(channel);
	if(i!=datasets.end())
	    i->second->show=on;
    }; 
/*
    // gibt zur�ck, ob der channel gerade geplottet wird 
    bool isVisible(const T& channel)
    {   typename dataset_map::iterator i = datasets.find(channel);
        if(i!=datasets.end())
            return i->second->show;
    }
*/
    /** switch showing of this channel off */
    void hide(const T& channel){ 
	show(channel, false);
    }; 

    /** switch showing all channels on or off */
    void show_all(bool on=true){
	for(typename dataset_map::iterator i=datasets.begin();i!=datasets.end();++i)
	    i->second->show=on;
    }; 

    /** switch showing all channels off */
    void hide_all(){ 
	show_all(false);
    }; 

    /** set title */
    void setTitle(const T& channel, const std::string& title){
	typename dataset_map::iterator i = datasets.find(channel);
	if(i!=datasets.end())
	    i->second->title=title;
    }; 

    /** set style */
    void setStyle(const T& channel, const std::string& style){
	typename dataset_map::iterator i = datasets.find(channel);
	if(i!=datasets.end())
	    i->second->style=style;
    }; 

    /** make gnuplot plot selected content of data buffers */
    void plot(int start, int end){
	// calculate real values for start and end
	int _start=buffersize-end;
	int _end=buffersize-start;
	fprintf(pipe,"plot ");
	bool first=true;
	for(typename dataset_map::iterator i=datasets.begin();i!=datasets.end();++i){
    	    Dataset* dataset=i->second;
	    if(dataset->show){
		if(first) first=false;
		else fprintf(pipe,", ");
		fprintf(pipe,"'-'");    
		fprintf(pipe," title '%s'",dataset->title.data());    
		fprintf(pipe," with %s",dataset->style.data());    
	    }
	}
	fprintf(pipe,"\n");
	for(typename dataset_map::iterator i=datasets.begin();i!=datasets.end();++i){
    	    Dataset* dataset=i->second;
	    if(dataset->show){
		dataset->plot(pipe,_start,_end);
	        fprintf(pipe,"e\n");
	    }
	}
	fflush(pipe);
    };    
    /** make gnuplot plot content of data buffers */
    void plot(){ plot(start,end); }
    

    /** set start of plot area 
	@param n steps in history (must be smaller than end)
    */
    void setStart(int n){ start=n; };

    /** set end of plot area 
	@param n steps in history (must be larger than start)
    */
    void setEnd(int n){ end=n; };

    /** returns buffersize */
    int getBuffersize() { return buffersize; };
    
    /** make gnuplot XY plot content of x against y data buffers 
	use it as follow:
	<pre>
	T x[]={a,b,c};
	T y[]={x,y,z};
	plotXY(x,y,3);
	</pre>
    */
    void plotXY(const T *x, const T *y,int size){
	fprintf(pipe,"plot ");
	bool first=true;
	for(int i=0;i < size; ++i){
	    typename dataset_map::iterator iX = datasets.find(x[i]);
	    typename dataset_map::iterator iY = datasets.find(y[i]);
	    // check if both channels exist
	    if(iX==datasets.end() || iY==datasets.end())
		continue;
	    if(first) first=false;
	    else fprintf(pipe,", ");
    	    Dataset* datasetX=iX->second;
    	    Dataset* datasetY=iY->second;
	    fprintf(pipe,"'-'");    
	    fprintf(pipe," title '%s'",std::string(datasetX->title + "<->" + datasetY->title).data());    
	    fprintf(pipe," with %s",datasetY->style.data());    
	}
	fprintf(pipe,"\n");
	for(int i=0;i < size; ++i){
	    typename dataset_map::iterator iX = datasets.find(x[i]);
	    typename dataset_map::iterator iY = datasets.find(y[i]);
	    // check if both channels exist
	    if(iX==datasets.end() || iY==datasets.end())
		continue;
	    for(int i2=0; i2<buffersize; ++i2)
		fprintf(pipe,"%f %f\n",iX->second->getData(i2),iY->second->getData(i2));
	    fprintf(pipe,"e\n");
	}
	fflush(pipe);
    };    

    /** make gnuplot XY plot content of x against y data buffers */
    void plotXY(const T& x, const T& y){
	plotXY(&x,&y,1);
    };    

    /** save content of all data buffers in files 
	( file names are: name + channel_name + ext )  */
    void printToFile(const std::string name="", const std::string ext=".log"){
	for(typename dataset_map::iterator i=datasets.begin();i!=datasets.end();++i){
	    std::string name2 = name + i->second->title + ext;
	    FILE* f=fopen(name2.data(),"w");    
	    i->second->plot(f);
	    fflush(f);
	    fclose(f);
	}
    };
};

#endif

