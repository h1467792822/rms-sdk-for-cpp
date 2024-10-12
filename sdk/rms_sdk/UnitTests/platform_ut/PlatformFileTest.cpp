/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
*/

#include "PlatformFileTest.h"
#include "../../Platform/Logger/Logger.h"
#include "../../Platform/Filesystem/IFile.h"
#include "../../Common/CommonTypes.h"
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;
using namespace rmscore::platform::filesystem;
using namespace rmscore::common;

void PlatformFileTest::testRead()
{
    string path1 = string(SRCDIR) + "data/tmpread.txt";
    ofstream file(path1);
    streampos start = file.tellp();

    string outdata = "This is test data stream !#$%";
    file.write(outdata.data(), (int)outdata.length());
    streampos end = file.tellp();
    streamsize wn = end - start;
    file.close();

    auto pFile = IFile::Create(path1, FileOpenModes::FILE_OPEN_READ);
    QVERIFY(pFile!=nullptr);

    CharArray bytes;
    auto rn = pFile->Read(bytes, wn);
    pFile->Close();
    auto ok = filesystem::remove(path1);
    QVERIFY(ok);

    std::string indata(bytes.data(), rn);

    QCOMPARE(outdata, indata);
}
void PlatformFileTest::testWrite()
{
    string path1 = string(SRCDIR) + "data/tmpread.txt";
    auto pFile = IFile::Create(path1, FileOpenModes::FILE_OPEN_WRITE);
    QVERIFY(pFile!=nullptr);

    std::string outdata = "This is test data stream !#$%";
    auto wn = pFile->Write(outdata.data(), outdata.length());
    pFile->Close();

    ifstream file(path1);
    CharArray bytes(wn);
    file.read(bytes.data(), (int)wn);
    streamsize rn = file.gcount();
    
    file.close();
    auto ok = filesystem::remove(path1);
    QVERIFY(ok);

    std::string indata(bytes.data(), rn);
    QCOMPARE(outdata, indata);
}
void PlatformFileTest::testClear()
{
    string path1 = string(SRCDIR) + "data/tmpclear.txt";
    auto pFile = IFile::Create(path1, FileOpenModes::FILE_OPEN_WRITE);
    QVERIFY(pFile!=nullptr);

    std::string outdata = "This is test data stream !#$%";
    auto wn = pFile->Write(outdata.data(), outdata.length());
    auto size = pFile->GetSize();
    QVERIFY(size==wn);

    pFile->Clear();
    auto size1 = pFile->GetSize();
    QVERIFY(size1==0);
    pFile->Close();
    auto ok = filesystem::remove(path1);
    QVERIFY(ok);
}
void PlatformFileTest::testReadAllAsText()
{
    string path1 = string(SRCDIR) + "data/testreadall.txt";
    auto pFile = IFile::Create(path1, FileOpenModes::FILE_OPEN_READ);
    QVERIFY(pFile!=nullptr);
    auto text = pFile->ReadAllAsText();
    pFile->Close();
    ifstream file(path1);
    string expectedText((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
   
    QCOMPARE(text, expectedText);
    file.close();
}
void PlatformFileTest::testAppendText()
{
    string path1 = string(SRCDIR) + "data/tmpappend.txt";
    auto pFile = IFile::Create(path1, FileOpenModes::FILE_OPEN_WRITE);
    QVERIFY(pFile!=nullptr);


    std::string text;
    std::stringstream ss;

    ss << "line 1" << std::endl << "line 2\n" << "line 3" << std::endl;
    text += ss.str();
    auto str = ss.str();
    pFile->AppendText(std::vector<uint8_t>(str.begin(), str.end()));
    ss.str("");

    ss << "line 4" << std::endl << "\tline 5" << std::endl << "line 6" << std::endl;
    text += ss.str();
    str = ss.str();
    pFile->AppendText(std::vector<uint8_t>(str.begin(), str.end()));
    ss.str("");

    ss << "line 7" << std::endl << "line 8" << std::endl << "line 9" << std::endl;
    text += ss.str();
    str = ss.str();
    pFile->AppendText(std::vector<uint8_t>(str.begin(), str.end()));
    ss.str("");

    pFile->Close();
    ifstream file(path1);
    string expectedText((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
    file.close();
    QCOMPARE(text, expectedText);
    auto ok = filesystem::remove(path1);
    QVERIFY(ok);
}
void PlatformFileTest::testGetSize()
{
    string path1 = string(SRCDIR) + "data/testsize.h";
    auto pFile = IFile::Create(path1, FileOpenModes::FILE_OPEN_READ);
    QVERIFY(pFile!=nullptr);
    auto size = pFile->GetSize();
    filesystem::path filePath(path1);

    auto expectedSize = (size_t)filesystem::file_size(filePath);
    QCOMPARE(size, expectedSize);
}


