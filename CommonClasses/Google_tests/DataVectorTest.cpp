#include "gtest/gtest.h"
#include "../DataVector/DataVector.h"

//Example
//EXPECT_EQ (9.0, (3.0*2.0)); // fail, test continues
//ASSERT_EQ (0.0, (0.0));     // success
//ASSERT_EQ (9, (3)*(-3.0));  // fail, test interrupts
//ASSERT_EQ (-9, (-3)*(-3.0));// not executed due to the previous assert

//struct DataVectorTest : testing::Test{
//    DataVector_lib* dataVector;
//    DataVector_lib* dataVector1;
//    std::vector<int> vector {4, 5, 6};
//    std::vector<int> vector1 {5, 6, 7};
//
//    DataVectorTest(){
//
//        dataVector = new DataVector_lib("id", vector );
//        dataVector1 = new DataVector_lib("id1", vector1);
//    }
//
//    ~DataVectorTest(){
//        delete dataVector;
//        delete dataVector1;
//    }
//};

TEST(DataVectorTest, ManhattanDistance) {
    std::vector<int> vector{4, 5, 6};
    std::vector<int> vector1{5, 6, 7};

    DataVector dataVector("id", vector);
    DataVector dataVector1("id1", vector1);

    EXPECT_EQ (dataVector.ManhattanDist(dataVector1), 3);

    dataVector.addComponent(0);
    dataVector1.addComponent(5);

    EXPECT_EQ (dataVector.ManhattanDist(dataVector1), 8);

    dataVector.addComponent(-5);
    dataVector1.addComponent(0);

    EXPECT_EQ (dataVector.ManhattanDist(dataVector1), 13);

    dataVector.addComponent(-5);
    dataVector1.addComponent(-3);

    EXPECT_EQ (dataVector.ManhattanDist(dataVector1), 15);

}


int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}