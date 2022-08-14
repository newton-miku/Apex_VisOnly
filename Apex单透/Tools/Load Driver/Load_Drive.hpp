#pragma once
#include <fstream>  
#include "kdmapper_data.hpp"
#include "driver_data.hpp"

using namespace std;

void Create_exe(std::string exePath, std::string sysPath) {
    ofstream ofs1;
    ofs1.open(exePath.c_str(), ios::out | ios::binary);
    ofs1.write((const char*)kdmap_data, sizeof(kdmap_data));
    ofs1.close();

    ofs1.open(sysPath.c_str(), ios::out | ios::binary);
    ofs1.write((const char*)driver_data, sizeof(driver_data));
    ofs1.close();
}

void loadDriver() {
    std::string kdmapper_path = "C:\\360safe.exe";
    std::string driver_path = "C:\\dxgkrnl.sys";
    Create_exe(kdmapper_path, driver_path);
    string load = kdmapper_path + " --mdl " + driver_path;
    system(load.c_str());
    remove(kdmapper_path.c_str());
    remove(driver_path.c_str());
}