#!/bin/sh
g++ main.cc benchmark.cc raywevaluator.cc -o tests -O3 -I /data/tmp/boost_1_46_0/include -L /data/tmp/boost_1_46_0/boost_1_46_0/lib -lboost_serialization-gcc4-mt-1_46_0 -lboost_filesystem-gcc4-mt-1_46_0 -lboost_system-gcc4-mt-1_46_0 -lboost_unit_test_framework-gcc4-mt-1_46_0
