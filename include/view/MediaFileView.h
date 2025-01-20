#ifndef MEDIAFILEVIEW_H
#define MEDIAFILEVIEW_H
#include "common/MenuRenderer.h"
#include "common/BaseView.h"
#include <vector>
#include <string>
#include <ftxui/component/component.hpp>  
#include <ftxui/component/screen_interactive.hpp>  
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using namespace ftxui;

class MediaFileView : public BaseView {
public:
    
    
    int showMenu() override; 
    void displayMediaFiles(const vector<string>& medialist, int page); 
    void displayPagination(int currentPage, int totalPages); 
    string promptDirectoryInput(); 
    
    
    int showOptionScan(); 
};

#endif 
