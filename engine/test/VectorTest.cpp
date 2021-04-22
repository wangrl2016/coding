#include <vector>
#include "gtest/gtest.h"
#include "include/Types.h"

/**
 * 对std::vector进行测试
 * 参考文档https://en.cppreference.com/w/cpp/container/vector
 */

struct Score {
    Score(int s) : score(s) {
        PRINT_FUNC();
    }

    ~Score() {
        PRINT_FUNC();
    }

    int score;
};

class Student {
public:
    void addScore(const Score& score) {
        scores.push_back(score);
    }

    void addScore(Score *pScore) {
        pScores.push_back(pScore);
    }

private:
    std::vector<Score> scores;
    std::vector<Score *> pScores;
};


TEST(Vector, Vector) {
    // 简单测试
    std::vector<int> v = {7, 5, 16, 8};
    v.push_back(25);
    v.push_back(13);
    ASSERT_EQ(v.size(), 6);

    // vector在类中测试
    auto *student = new Student;
    {
        Score score(50);
        student->addScore(score);
        auto *pScore = new Score(50);
        student->addScore(pScore);
    }
    printf("TEST END\n");
    delete student;
}
