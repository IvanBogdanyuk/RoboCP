#pragma once
#include "XMLConfig.cpp"
#include "CommandMaker.cpp"
#include "KinectViewer.cpp"

#include <boost/thread.hpp>
#include <boost/bind.hpp>


int main () {

	XMLConfig x;
	{ //deserialization
        std::ifstream ifs("config.xml");
		boost::archive::xml_iarchive ia(ifs);
        ia >> BOOST_SERIALIZATION_NVP(x);
	}

	CommandMaker c (x);
	KinectViewer k (x);
	
	boost::thread_group tgroup;

	tgroup.create_thread ( boost::bind (&CommandMaker::run, &c) );

	tgroup.create_thread ( boost::bind (&KinectViewer::run, &k) );


	tgroup.join_all ();
	return 0;
}