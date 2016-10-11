/*
 * Copyright 2014 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define FLATBUFFERS_DEBUG_VERIFICATION_FAILURE 1
#define FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE

#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/idl.h"
#include "flatbuffers/util.h"

#include "monster_test_generated.h"
#include "namespace_test/namespace_test1_generated.h"
#include "namespace_test/namespace_test2_generated.h"
#include "student_definition_generated.h"

#ifndef FLATBUFFERS_CPP98_STL

#include <random>
#include <iostream>
#include <chrono>
#include <msgpack.hpp>
#include <string>
#include <sstream>

#endif

#include "flatbuffers/schemaless.h"

using namespace MyGame::Example;

#ifdef __ANDROID__
#include <android/log.h>
#define TEST_OUTPUT_LINE(...) \
    __android_log_print(ANDROID_LOG_INFO, "FlatBuffers", __VA_ARGS__)
#define FLATBUFFERS_NO_FILE_TESTS
#else
#define TEST_OUTPUT_LINE(...) \
    { printf(__VA_ARGS__); printf("\n"); }
#endif

int testing_fails = 0;

void TestFail(const char *expval, const char *val, const char *exp,
              const char *file, int line) {
    TEST_OUTPUT_LINE("TEST FAILED: %s:%d, %s (%s) != %s", file, line,
                     exp, expval, val);
    assert(0);
    testing_fails++;
}

void TestEqStr(const char *expval, const char *val, const char *exp,
               const char *file, int line) {
    if (strcmp(expval, val) != 0) {
        TestFail(expval, val, exp, file, line);
    }
}

template<typename T, typename U>
void TestEq(T expval, U val, const char *exp, const char *file, int line) {
    if (U(expval) != val) {
        TestFail(flatbuffers::NumToString(expval).c_str(),
                 flatbuffers::NumToString(val).c_str(),
                 exp, file, line);
    }
}

#define TEST_EQ(exp, val) TestEq(exp,         val,   #exp, __FILE__, __LINE__)
#define TEST_NOTNULL(exp) TestEq(exp == NULL, false, #exp, __FILE__, __LINE__)
#define TEST_EQ_STR(exp, val) TestEqStr(exp,  val,   #exp, __FILE__, __LINE__)


void PrintSlb(schemaless::Builder &slb) {
    for (size_t i = 0; i < slb.GetBuffer().size(); i++)
        printf("%d ", slb.GetBuffer().data()[i]);
    printf("\n");
}

void PrintBuf(std::vector<uint8_t> buf) {
    printf("\n-------->\n");
    printf("%10s", "Decimal: ");
    for (size_t i = 0; i < buf.size(); i++)
        printf("%-4d", buf[i]);
    printf("\n");

//    printf("%10s", "Char: ");
//    for (size_t i = 0; i < buf.size(); i++)
//        printf("%-4c", buf[i]);
//    printf("\n");

    printf("%10s", "Hexa: ");
    for (size_t i = 0; i < buf.size(); i++)
        printf("%-4X", buf[i]);
    printf("\n<--------\n");
}

std::pair<uint64_t, const std::vector<uint8_t>> BuildSchemaless() {
    schemaless::Builder slb;
    slb.Map([&]() {
        slb.Key("name");
        slb.String("Pham Van Thong");

        slb.Key("birth_year");
        slb.UInt(1995);

        slb.Key("male");
        slb.UInt(1); //Doesn't support boolean

        slb.Key("scores");
        slb.Vector([&]() {
            slb.Map([&]() {
                slb.Key("subject");
                slb.String("Math");
                slb.Key("score");
                slb.UInt(7);
            });
            slb.Map([&]() {
                slb.Key("subject");
                slb.String("Physics");
                slb.Key("score");
                slb.UInt(8);
            });
        });

        slb.Key("avg_scores");
        slb.Map([&]() {
            slb.Key("subject");
            slb.String("Average score");
            slb.Key("score");
            slb.Float(7.5);
        });

        slb.Key("school_day");
        slb.Vector([&]() {
            slb.UInt(2);
            slb.UInt(3);
            slb.UInt(5);
            slb.UInt(7);
        });

    });

    slb.Finish();

    return std::make_pair(slb.GetBuffer().size(), slb.GetBuffer());
}

const std::vector<uint8_t> BuildMapFailed() {
    schemaless::Builder slb;
    slb.Map([&]() {
        slb.Key("name");
        slb.String("pham van thong");

        slb.Key("tag");
        slb.Int(-2);
//todo: swap tag and age then schemaless builder will go wrong
        slb.Key("age");
        slb.UInt(22);

    });
    slb.Finish();

    return slb.GetBuffer();
}


///TEST FUNCTION-------------------->
const std::vector<uint8_t> BuildUntypedVector() {
    schemaless::Builder slb;
    slb.Vector([&]() {
        slb.Int(-4);
        slb.String("Hello");
        slb.UInt(1995);
    });
    slb.Finish();
    return slb.GetBuffer();
}

const std::vector<uint8_t> BuildTypedVector() {
    schemaless::Builder slb;
    slb.TypedVector([&]() {
        slb.UInt(8);
        slb.UInt(16);
        slb.UInt(1995);
        slb.UInt(255);
    });
    slb.Finish();
    return slb.GetBuffer();
}

const std::vector<uint8_t> BuildMap() {
    schemaless::Builder slb;
    slb.Map([&]() {
        slb.Key("reverse1");
        slb.String("5000d");

        slb.Key("reverse2");
        slb.String("5000d");

        slb.Key("reverse3");
        slb.String("5000d");

        slb.Key("reverse4");
        slb.String("5000d");

        slb.Key("reverse5");
        slb.String("5000d");

        slb.Key("reverse6");
        slb.String("5000d");

        slb.Key("reverse7");
        slb.String("5000d");

        slb.Key("reverse8");
        slb.String("5000d");

        slb.Key("reverse9");
        slb.String("5000d");

    });
    slb.Finish();

    return slb.GetBuffer();
}


/*
  Cust {
  CustID = "1234",

  lstmembership = [{
    membershipId="4321"
    membershipName = "Member ship name"
  }, {
    membershipId="4321"
    membershipName = "Member ship name"
  }, ...],

  lstBalance = [{
    balTypeID = "1",
    consume = "1000d",
    reverse = "5000d",
    expDate = "12345678"
  },.....]

  custDetail = {
    custId = "1234",
    email = "abc@gmail.com",
    name = "cust name"
  }
}
*/

const std::vector<uint8_t> BuildTestData() {
    static schemaless::Builder *slb;
    if (slb != NULL)
        delete slb;
    slb = new schemaless::Builder;
    slb->Map([&]() {
        slb->Key("custId");
        slb->String("1234");

        slb->Key("lstMembership");
        slb->Vector([&]() {
            slb->Map([&]() {
                slb->Key("membershipId");
                slb->String("1");
            });

            slb->Map([&]() {
                slb->Key("membershipId");
                slb->String("2");
            });

            slb->Map([&]() {
                slb->Key("membershipId");
                slb->String("3");
            });

            slb->Map([&]() {
                slb->Key("membershipId");
                slb->String("4");
            });

            slb->Map([&]() {
                slb->Key("membershipId");
                slb->String("5");
            });

            slb->Map([&]() {
                slb->Key("membershipId");
                slb->String("6");
            });
        });

        slb->Key("lstBalance");
        slb->Vector([&]() {
            slb->Map([&]() {
                slb->Key("balTypeId");
                slb->String("12");

                slb->Key("consume");
                slb->String("1000d");

                slb->Key("current");
                slb->Int(100000);
            });

            slb->Map([&]() {
                slb->Key("balTypeId");
                slb->String("23");

                slb->Key("consume");
                slb->String("2000d");

                slb->Key("current");
                slb->Int(100000);
            });

            slb->Map([&]() {
                slb->Key("balTypeId");
                slb->String("34");

                slb->Key("consume");
                slb->String("3000d");

                slb->Key("current");
                slb->Int(100000);
            });

            slb->Map([&]() {
                slb->Key("balTypeId");
                slb->String("45");

                slb->Key("consume");
                slb->String("4000d");

                slb->Key("current");
                slb->Int(100000);
            });

            slb->Map([&]() {
                slb->Key("balTypeId");
                slb->String("56");

                slb->Key("consume");
                slb->String("5000d");

                slb->Key("current");
                slb->Int(100000);
            });

            slb->Map([&]() {
                slb->Key("balTypeId");
                slb->String("67");

                slb->Key("consume");
                slb->String("6000d");

                slb->Key("current");
                slb->Int(100000);
            });
        });


        slb->Key("province");
        slb->Int(64);
    });
    slb->Finish();

    return slb->GetBuffer();
}










#define BUILD_COUNT 1000
#define READ_COUNT 10000
#define UPDATE_COUNT 10000
#define TEST_COUNT 2

#define TEST
//#undef TEST

#ifdef TEST
#define PRINT_WHILE_BUILDING
#define SIZE_OF_VECTOR 100
//define max random value
#define MAX_RAND_VALUE  1000
#endif

#ifndef TEST
#define SIZE_OF_VECTOR 1000
//define max random value
#define MAX_RAND_VALUE INT32_MAX
#endif

//utils function
long GetCurrentMiliseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
}

int GetRandomValue(int max_value) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, INT32_MAX);
    return dis(gen) % max_value;
}


#define NUMBER_OF_MEMBERSHIP 10
#define NUMBER_OF_GROUP 10
#define NUMBER_OF_BALANCE 10

const std::vector<uint8_t> & BuildCustSchemaless() {
    static schemaless::Builder * slb = NULL;
    static int index=0;
    if (slb != NULL)
        delete(slb);
    slb = new schemaless::Builder();
    slb->Map([&]() {
        slb->Key("custId");
        slb->Int(++index);

        slb->Key("lstMembership");
        slb->Vector([&]() {
            for (int i=0; i<NUMBER_OF_MEMBERSHIP; i++)
                slb->Map([&]() {
                    slb->Key("membershipId");
                    slb->Int(i);
                    slb->Key("masterCustId");
                    slb->Int(1);
                    slb->Key("expDate");
                    slb->String("10/10/2010");
                });
        });


        slb->Key("lstBalance");
        slb->Vector([&]() {
            for (int i=0; i<NUMBER_OF_BALANCE; i++) {
                slb->Map([&]() {
                    slb->Key("balTypeId");
                    slb->Int(i);
                    slb->Key("level");
                    slb->Int(i);
                    slb->Key("balOf");
                    slb->Int(87);
                    slb->Key("reverse");
                    slb->Int(23);
                    slb->Key("consume");
                    slb->Int(45);
                    //todo problem when values is an integer
                    slb->Key("expDate");
                    slb->String("10/10/2012");
                });
            }
        });

        slb->Key("lstGroup");
        slb->Vector([&]() {
            for (int i=0; i<NUMBER_OF_GROUP; i++)
                slb->Map([&](){
                    slb->Key("groupId");
                    slb->Int(i);
                    slb->Key("groupType");
                    slb->Int(i);
                    slb->Key("expDate");
                    slb->String("10/10/2011");

                    slb->Key("expDate111");
                    slb->String("10/10/2011111");
                });
        });


        slb->Key("custDetail");
        slb->Map([&]() {
            slb->Key("firstname");
            slb->String("Thong");
            slb->Key("lastname");
            slb->String("Pham Van");
            slb->Key("email");
            slb->String("thongpv87@gmail.com");
            slb->Key("state");
            slb->Int(5);
            slb->Key("expDate");
            slb->String("10/10/2013");
        });
    });

    slb->Finish();
    return slb->GetBuffer();
}

void TestCustSchemaless() {
    const std::vector<uint8_t> buf = BuildCustSchemaless();
    auto custMap = schemaless::GetRoot(buf).AsMap();

    auto custId = custMap["custId"].AsInt32();
    auto lstMembershipVector = custMap["lstMembership"].AsVector();
    auto lstGroupVector = custMap["lstGroup"].AsVector();
    auto lstBalanceVector = custMap["lstBalance"].AsVector();
    auto custDetailMap = custMap["custDetail"].AsMap();

    printf("\n\nRead Cust schemaless buff====>\n");
    printf("\nCustId = %d", custId);


    printf("\n\nVector of membership: ");
    printf("\n[");
    for (int i=0; i<NUMBER_OF_MEMBERSHIP; i++) {
        auto membership = lstMembershipVector[i].AsMap();
        printf("\n\t{membershipId = %d, masterCustId = %d, expDate = \"%s\"},",
               membership["membershipId"].AsInt32(), membership["masterCustId"].AsInt32(),
               membership["expDate"].AsString().c_str());
    }
    printf("\n]\n");

    printf("\n\nVector of group: ");
    printf("\n[");
    for (int i=0; i<NUMBER_OF_GROUP; i++) {
        auto group = lstGroupVector[i].AsMap();
        printf("\n\t{groupId = %d, groupType = %d, expDate = \"%s\"},",
               group["groupId"].AsInt32(), group["groupType"].AsInt32(),
               group["expDate"].AsString().c_str());
    }
    printf("\n]\n");

    printf("\n\nVector of balance: ");
    printf("\n[");
    for (int i=0; i<NUMBER_OF_BALANCE; i++) {
        auto balance = lstBalanceVector[1].AsMap();
        printf("\n\t{balTypeId = %d, balOf = %d, level = %d, consume = %d, reverse = %d, expDate = \"%s\"},",
               balance["balTypeId"].AsInt32(),balance["balOf"].AsInt32() , balance["level"].AsInt32(), balance["consume"].AsInt32(),
            balance["reverse"].AsInt32(), balance["expDate"].AsString().c_str());
    }
    printf("\n]\n");

    printf("\n\nCustDetail: \n\t{\"%s\", \"%s\", \"%s\", %d, \"%s\"}", custDetailMap["firstname"].AsString().c_str(), custDetailMap["lastname"].AsString().c_str(),
        custDetailMap["email"].AsString().c_str(), custDetailMap["state"].AsInt32(), custDetailMap["expDate"].AsString().c_str());
}

//============COMPLETED FUNCTION======================
const std::vector<uint8_t> & BuildTypedIntVectorSchemaless() {
    static schemaless::Builder *slb = NULL;
    if (slb != NULL)
        delete slb;
    slb = new schemaless::Builder;

#ifdef  PRINT_WHILE_BUILDING
    printf("\nData of schemaless buff====>\n");
#endif

    slb->TypedVector([&]() {
        int t;
        for (int i = 0; i < SIZE_OF_VECTOR; i++) {
            t = GetRandomValue(MAX_RAND_VALUE);
            slb->Int(t);

#ifdef  PRINT_WHILE_BUILDING
            printf("\n[%d] = %15d", i, t);
#endif
        }
    });
    slb->Finish();
    return slb->GetBuffer();
}

msgpack::sbuffer * BuildTypedIntVectorMsgpack() {
    std::vector<int> vec;
#ifdef  PRINT_WHILE_BUILDING
    printf("\nData of msgpack buff====>\n");
#endif
    int t;
    for (int i = 0; i < SIZE_OF_VECTOR; i++) {
        t = GetRandomValue(MAX_RAND_VALUE);
        vec.push_back(t);
#ifdef  PRINT_WHILE_BUILDING
        printf("\n[%d] = %15d", i, t);
#endif
    }


    static msgpack::sbuffer * sbuf = NULL;
    if (sbuf != NULL)
        delete sbuf;
    sbuf = new msgpack::sbuffer();
    msgpack::pack(*sbuf, vec);
    return sbuf;
}

void ReadTypedIntVectorSchemaless(const std::vector<uint8_t> &buf) {
    auto vec = schemaless::GetRoot(buf).AsVector();
    int t;
    for (int i = 0; i < READ_COUNT; i++) {
        t = GetRandomValue(MAX_RAND_VALUE);
        vec[t].AsInt32();
    }
}

void ReadTypedIntVectorMsgpack(msgpack::sbuffer *sbuf) {
    msgpack::object obj = msgpack::unpack(sbuf->data(), sbuf->size()).get();
    std::vector<int> vec;
    obj.convert(vec);
    int t;
    for (int i = 0; i < READ_COUNT; i++) {
        t = GetRandomValue(MAX_RAND_VALUE);
        vec[t];
    }
}

//Test OK
void TestTypedIntVectorMsgpack() {
    msgpack::sbuffer *sbuf = BuildTypedIntVectorMsgpack();
    msgpack::object obj = msgpack::unpack(sbuf->data(), sbuf->size()).get();
    std::vector<int> vec;
    obj.convert(vec);
    int t;
    printf("\n\nRead msgpack buff====>\n");
    for (int i = 0; i < TEST_COUNT; i++) {
        t = GetRandomValue(SIZE_OF_VECTOR);
        printf("\n[%d] = %15d", t, vec[t]);
    }
}
//Test OK
void TestTypedIntVectorSchemaless() {
    std::vector<uint8_t> buf = BuildTypedIntVectorSchemaless();
    auto vec = schemaless::GetRoot(buf).AsTypedVector();
    int t;
    printf("\n\nRead of schemaless buff====>\n");
    for (int i = 0; i < TEST_COUNT; i++) {
        t = GetRandomValue(SIZE_OF_VECTOR);
        printf("\n[%d] = %15d", t, vec[t].AsInt32());
    }
}


enum operation {
    BUILD, READ, UPDATE
};

void RunBenchmark(operation type) {
    long t1, t2;
    switch (type) {
        case BUILD: {
            printf("\n\n==============BUILD BENCHMARK====================");
            t1 = GetCurrentMiliseconds();
            for (int i = 0; i < BUILD_COUNT; i++)
                BuildTypedIntVectorSchemaless();
            t2 = GetCurrentMiliseconds();
            printf("\nTyped integer schemaless BUILD time: %ld", t2 - t1);

            t1 = GetCurrentMiliseconds();
            for (int i = 0; i < BUILD_COUNT; i++)
                BuildTypedIntVectorSchemaless();
            t2 = GetCurrentMiliseconds();
            printf("\nTyped integer msgpack BUILD time: %ld", t2 - t1);
            break;
        }
        case READ: {
            printf("\n\n==============READ BENCHMARK====================");
            t1 = GetCurrentMiliseconds();
            const std::vector<uint8_t> buf1 = BuildTypedIntVectorSchemaless();
            for (int i = 0; i < READ_COUNT; i++)
                ReadTypedIntVectorSchemaless(buf1);
            t2 = GetCurrentMiliseconds();
            printf("\nTyped integer schemaless READ time: %ld", t2 - t1);
            msgpack::sbuffer * buf2;
            buf2 = BuildTypedIntVectorMsgpack();
            t1 = GetCurrentMiliseconds();
            for (int i = 0; i < READ_COUNT; i++)
                ReadTypedIntVectorMsgpack(buf2);
            t2 = GetCurrentMiliseconds();
            printf("\nTyped integer msgpack READ time: %ld", t2 - t1);
            break;
        }
        case UPDATE:
        {
            printf("\n\n==============UPDATE BENCHMARK====================");
            break;
        }
        default:
            break;
    }
}

void RunTest() {
    printf("\nTest typed vector schemlaless ----> \n");
    TestTypedIntVectorSchemaless();
    printf("\n\n=========================\nTest typed vector msgpack ----> \n");
    TestTypedIntVectorMsgpack();
    printf("\n\n=========================\nTest Cust schemaless ----> \n");
    TestCustSchemaless();
}

int main() {
#ifdef TEST
    RunTest();
#else
    RunBenchmark(READ);
#endif
}