//
// Created by zhaojieyi on 2023/1/16.
//

#include "declarative_struct.h"
#include "gtest/gtest.h"

MTAG_DEFINE_BEGIN(LatencyTag)
MTAG_DEFINE(pod_name, my_pod)
MTAG_DEFINE(cluster, my_cluster)
MTAG_DEFINE_END()

struct SimpleStruct {
  int int_;
  std::string string_;
};

DEFINE_STRUCT_SCHEMA(
    SimpleStruct,
    DEFINE_STRUCT_FIELD(int_, "int"),
    DEFINE_STRUCT_FIELD(string_, "string"));

TEST(TestDeclarativeStruct, TestConstruct) {
  LatencyTag tag;
  std::cout << tag.pod_name  << " | " << tag.cluster << std::endl;

  ForEachField(SimpleStruct{1, "hello static reflection"},
               [](auto&& field, auto&& name) {
                 std::cout << name << ": "
                           << field << std::endl;
               });
}