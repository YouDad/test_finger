#include "stdafx.h"

// 1~8小按钮的点击事件
void MainDialog::OnBnClickedBtn(){
    // 设置指纹号的文本
    setText(editFingerId,getText(GetFocus()));
}

// 注册按钮事件
void MainDialog::OnBnClickedBtnEnroll(){
    if(getText(cmbProtocolType)==SYNO){
        // 判空检查
        if(getText(editFingerId)==""){
            MyLog::user("编号不能为空");
            return;
        }
        // static 防止被释放
        static uint8_t BufferID=1;
        // 获得指纹号
        uint8_t FingerID=MyString::ParseInt(getText(editFingerId));
        static const int EnrollCount=3;
        // 定义流程
        flow.clear();
        // 流程 0:发送<获取注册图像>命令
        flow.add(0,
            [](int& result)->bool{
                MainDialogCtrlValidity::Working();
                comm.request(SII(GetEnrollImage));
                setProgress(100*flow.percent());
                flow.next();
                return false;
            }
        );
        // 流程 1:如果取到图像,发送<上传图像>命令
        // 流程 1:如果没取到图像,回到流程 0
        flow.add(1,
            [](int& result)->bool{
                if(result==0x00){
                    // 取到图像
                    MyLog::debug("取到指纹图像(%d/%d)",BufferID,EnrollCount);
                    comm.request(SII(UpImage));//上传图像
                    setProgress(100*flow.percent());
                    flow.next();
                    return false;
                } else{//没取到图像
                    flow.prev();
                    return true;
                }
            }
        );
        // 流程 2:如果上传图像成功,发送<生成特征>命令
        // 流程 2:如果上传图像失败,就注册失败了
        flow.add(2,
            [](int& result)->bool{
                if(result==0x00){
                    // 上传图像成功
                    comm.request(SII(GenChar),DataPacket(&BufferID,1));
                    setProgress(100*flow.percent());
                    flow.next();
                    return false;
                } else{
                    // 上传图像失败
                    MyLog::debug("上传图像失败");
                    MyLog::user("注册失败");
                    flow.terminate();
                    return false;
                }
            }
        );
        // 流程 3:如果生成特征成功,发送<搜索指纹>命令
        // 流程 3:如果生成特征失败,从头开始采指纹
        flow.add(3,
            [](int& result)->bool{
                if(result==0x00){
                    // 生成特征成功
                    MyLog::debug("生成特征成功(%d/%d)",BufferID,EnrollCount);

                    uint8_t x[]={BufferID,0,0,0,0x87};
                    comm.request(SII(Search),DataPacket(x,sizeof x));
                    setProgress(100*flow.percent());
                    flow.next();
                    return false;
                } else{
                    // 生成特征失败
                    flow.jump(0);
                    return true;
                }
            }
        );
        // 流程 4:如果没有搜索到指纹,发送<获得指纹>命令
        // 流程 4:如果搜索到了指纹,代表已经注册过了,就注册失败了
        flow.add(4,
            [](int& result)->bool{
                if(result==0x09||result==0x17){
                    // 没搜到指纹
                    MyLog::debug("等待手指移开");
                    comm.request(SII(GetRawImage));
                    setProgress(100*flow.percent());
                    flow.next();
                    return false;
                } else{
                    // 搜到指纹
                    MyLog::user("该指纹注册过了,注册失败");
                    flow.terminate();
                    return true;
                }
            }
        );
        // 流程 5:如果没有手指,执行下一个流程
        // 流程 5:如果有手指,按照result=0x09的方式执行上一个流程
        //        即在等待手指移开
        flow.add(5,
            [](int& result)->bool{
                if(result==0x02){
                    // 没指纹
                    flow.next();
                } else{
                    // 有指纹
                    result=0x09;
                    flow.prev();
                }
                return true;
            }
        );
        // 流程 6:如果达到录入次数,执行下一个流程
        // 流程 6:如果没达到录入次数,录入计数+1,回流程0录入指纹
        flow.add(6,
            [](int& result)->bool{
                if(BufferID>=EnrollCount){
                    // 达到录入次数
                    flow.next();
                } else{
                    // 没达到录入次数
                    BufferID++;
                    flow.jump(0);
                }
                return true;
            }
        );
        // 流程 7:发送<合并特征>命令
        flow.add(7,
            [](int& result)->bool{
                comm.request(SII(RegModel));//合并特征
                setProgress(100*flow.percent());
                flow.next();
                return false;
            }
        );
        // 流程 8:如果合并特征成功,发送<存储模板>命令
        // 流程 8:如果合并特征失败,就注册失败了
        flow.add(8,
            [FingerID](int& result)->bool{
                if(result==0x00){
                    // 合并特征成功
                    MyLog::debug("合并特征成功");

                    uint8_t data[]={1,0,FingerID};
                    comm.request(SII(StoreChar),DataPacket(data,sizeof data));//存储模板
                    setProgress(100*flow.percent());
                    flow.next();
                    return false;
                } else{
                    // 合并失败
                    MyLog::user("注册失败");
                    flow.terminate();
                }
                return true;
            }
        );
        // 流程 9:如果存储模板成功,就报告一声
        // 流程 9:如果存储模板失败,就注册失败了
        flow.add(9,
            [](int& result)->bool{
                if(result==0x00){
                    // 存储模板成功
                    flow.next();
                    MyLog::debug("存储模板成功");
                } else{
                    // 存储失败
                    MyLog::user("注册失败");
                    flow.terminate();
                }
                return true;
            }
        );
        // 流程10:注册结束,善后工作
        flow.add(10,
            [](int& result)->bool{
                BufferID=1;
                setProgress(100*flow.percent());
                MyLog::user("注册结束");
                flow.clear();
                MainDialogCtrlValidity::Work();
                return false;
            }
        );
        // 开始执行流程
        flow.start();
    }
}

// 比对按钮点击事件
void MainDialog::OnBnClickedBtnMatch(){
    if(getText(cmbProtocolType)==SYNO){
        // 判空检查
        if(getText(editFingerId)==""){
            MyLog::user("编号不能为空");
            return;
        }
        // 获得指纹号
        uint8_t FingerID=MyString::ParseInt(getText(editFingerId));
        // 定义流程
        flow.clear();
        // 流程 0:发送<载入特征>命令
        flow.add(0,[FingerID](int& result)->bool{
            MainDialogCtrlValidity::Working();
            uint8_t data[]={02,00,FingerID};
            comm.request(SII(LoadChar),DataPacket(data,sizeof data));
            flow.next();
            MyLog::user("读出指纹模板中...");
            return false;
            });
        // 流程 1:如果载入特征成功,发送<获取图像>命令
        // 流程 1:如果载入特征失败,就比对失败了
        flow.add(1,[](int& result)->bool{
            if(result==0x00){
                comm.request(SII(GetRawImage));
                setProgress(100*flow.percent());
                flow.next();
                return false;
            } else{
                flow.terminate();
                return true;
            }
            });
        // 流程 2:如果取图成功,发送<生成特征>命令
        // 流程 2:如果取图失败,返回上一个流程
        flow.add(2,[](int& result)->bool{
            if(result==0x00){
                MyLog::user("取指纹图成功");
                uint8_t data[]={02};
                comm.request(SII(GenChar),DataPacket(data,sizeof data));
                setProgress(100*flow.percent());
                flow.next();
                return false;
            } else{
                result=0x00;
                flow.prev();
                return true;
            }
            });
        // 流程 3:如果生成特征成功,发送<匹配>命令
        // 流程 3:如果生成特征失败,回到流程 1,重新取图
        flow.add(3,[](int& result)->bool{
            if(result==0x00){
                MyLog::user("指纹生成特征成功");
                comm.request(SII(Match));
                setProgress(100*flow.percent());
                flow.next();
                return false;
            } else{
                MyLog::user("指纹生成特征失败");
                result=0x00;
                flow.jump(1);
                return true;
            }
            });
        // 流程 4:输出匹配结果
        flow.add(4,[](int& result)->bool{
            if(result==0x00){
                MyLog::user("指纹匹配成功");
            } else{
                MyLog::user("指纹不匹配");
            }
            flow.next();
            return true;
            });
        // 流程 5:比对结束,善后工作
        flow.add(5,[](int& result)->bool{
            setProgress(100*flow.percent());
            MyLog::user("比对结束");
            flow.clear();
            MainDialogCtrlValidity::Work();
            return false;
            });
        // 开始执行流程
        flow.start();
    }
}

// 查看所有注册指纹的ID的按钮点击事件
void MainDialog::OnBnClickedBtnViewEnrollIDs(){
    if(getText(cmbProtocolType)==SYNO){
        // 定义流程
        flow.clear();
        // 流程 0:发送<读取引索表>命令
        flow.add(0,[](int& result)->bool{
            MainDialogCtrlValidity::Working();
            uint8_t data[]={0};
            comm.request(SII(ReadIndexTable),DataPacket(data,sizeof data));
            setProgress(100*flow.percent());
            flow.next();
            return false;
            });
        // 流程 1:读取引索表结束,善后工作
        flow.add(1,[](int& result)->bool{
            setProgress(100*flow.percent());
            flow.clear();
            MainDialogCtrlValidity::Work();
            return false;
            });
        // 开始执行流程
        flow.start();
    }
}

// 删除模板的按钮点击事件
void MainDialog::OnBnClickedBtnDeleteTemplate(){
    if(getText(cmbProtocolType)==SYNO){
        // 判空检查
        if(getText(editFingerId)==""){
            MyLog::user("编号不能为空");
            return;
        }
        // 获得指纹号
        uint8_t FingerID=MyString::ParseInt(getText(editFingerId));
        // 定义流程
        flow.clear();
        // 流程 0:发送<删除模板>命令
        flow.add(0,[FingerID](int& result)->bool{
            MainDialogCtrlValidity::Working();
            uint8_t data[]={0,FingerID,0,1};
            comm.request(SII(DeleteChar),DataPacket(data,sizeof data));
            setProgress(100*flow.percent());
            flow.next();
            return false;
            });
        // 流程 1:删除模板结束,善后工作
        flow.add(1,[FingerID](int& result)->bool{
            if(result==0x00){
                MyLog::user("删除指纹%d成功",FingerID);
            }
            // 如果失败了,在监听器会给出错误原因
            setProgress(100*flow.percent());
            flow.clear();
            MainDialogCtrlValidity::Work();
            return false;
            });
        // 开始执行流程
        flow.start();
    }
}
