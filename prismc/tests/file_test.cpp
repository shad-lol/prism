
/*   Copyright 2026 Prism Compiler Authors
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <file/file.hpp>

#include <gtest/gtest.h>

#include <fstream>

int init1 = prismc::register_codes();
int init2 = prismc::diag.silent = true;

class FileTest : public ::testing::Test {
protected:
    std::filesystem::path temp_dir;
    std::filesystem::path valid_file_path;
    std::filesystem::path missing_file_path;
    std::string sample_content;

    void SetUp() override {
        temp_dir = std::filesystem::temp_directory_path() / "prismc_tests";
        std::filesystem::create_directories(temp_dir);

        valid_file_path = temp_dir / "sample.txt";
        missing_file_path = temp_dir / "ghost.txt";
        sample_content = "fn main() { return 0; }";

        std::ofstream out(valid_file_path);
        out << sample_content;
        out.close();
    }

    void TearDown() override {
        std::filesystem::remove_all(temp_dir);
    }
};

TEST_F(FileTest, LoadSuccess) {
    prismc::File file;
    uint16_t res = file.load(valid_file_path);

    EXPECT_EQ(res, prismc::err::Success);
    EXPECT_EQ(file.path, valid_file_path);
    EXPECT_EQ(file.code, sample_content);
}

TEST_F(FileTest, FileNotFound) {
    prismc::File file;

    EXPECT_EXIT(
        {
            file.load(missing_file_path);
        },
        ::testing::ExitedWithCode(0xF101),

        ""
    );
}

TEST_F(FileTest, AccessDenied) {
    prismc::File file;
    uint16_t res = file.load(temp_dir);

    EXPECT_EQ(res, prismc::err::ErrAccessDenied);
}

TEST_F(FileTest, EmptyFile) {
    std::filesystem::path empty_path = temp_dir / "empty.txt";
    std::ofstream out(empty_path);
    out.close();

    prismc::File file;
    uint16_t res = file.load(empty_path);

    EXPECT_EQ(res, prismc::err::Success);
    EXPECT_TRUE(file.code.empty());
}