#pragma once
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>



class Command {
private:
  friend class boost::serialization::access;
 
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
	ar & BOOST_SERIALIZATION_NVP(sec);
	ar & BOOST_SERIALIZATION_NVP(angle);
  }

public:
  int sec;
  float angle;
  Command (int s, float a);
  Command ();
};
