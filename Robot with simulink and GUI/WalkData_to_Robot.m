function [back] = WalkData_to_Robot(usart)
back=0;
MD_1=xlsread('SD_1');MD_2=xlsread('SD_2');MD_3=xlsread('SD_3');MD_4=xlsread('SD_4');MD_5=xlsread('SD_5');
MD_6=xlsread('SD_6');MD_7=xlsread('SD_7');MD_8=xlsread('SD_8');MD_9=xlsread('SD_9');MD_10=xlsread('SD_10');
MD_11=xlsread('SD_11');MD_12=xlsread('SD_12');MD_13=xlsread('SD_13');MD_14=xlsread('SD_14');MD_15=xlsread('SD_15');
MD_16=xlsread('SD_16');MD_17=xlsread('SD_17');MD18_=xlsread('SD_18');MD_19=xlsread('SD_19');MD_20=xlsread('SD_20');
MD_21=xlsread('SD_21');MD_22=xlsread('SD_22');MD_23=xlsread('SD_23');
Send_Data='storage_dance:A1:%6.2f,A2:%6.2f,A3:%6.2f,A4:%6.2f,A5:%6.2f,A6:%6.2f,A7:%6.2f,A8:%6.2f,A9:%6.2f,A10:%6.2f,A11:%6.2f,A12:%6.2f,A13:%6.2f,A14:%6.2f,A15:%6.2f,A16:%6.2f,A17:%6.2f,A18:%6.2f,A19:%6.2f,A20:%6.2f,A21:%6.2f,A22:%6.2f,A23:%6.2f&&\n';
for i=1:90
    SEND=sprintf(Send_Data,MD_1(1,i),MD_2(1,i),MD_3(1,i),MD_4(1,i),MD_5(1,i),MD_6(1,i),MD_7(1,i),MD_8(1,i),MD_9(1,i),MD_10(1,i),MD_11(1,i),MD_12(1,i),MD_13(1,i),MD_14(1,i),MD_15(1,i),MD_16(1,i),MD_17(1,i),MD_18(1,i),MD_19(1,i),MD_20(1,i),MD_21(1,i),MD_22(1,i),MD_23(1,i));
    USART_Send(usart,SEND);
end
USART_Send(usart,"storage:over");

%%  ����˼·    ���Կ�ʼѭ������  Robot������׼��23��������н���   ����һ�ε�ַ��1ֱ����⵽��������storage���ַ��������⵽OVER
%%  ��⵽over�����  �������з���   ����ǲ���ָ������  �����ݴ�������1~23
%%                                      �¶�ָ��      �����ݴ�������31~53
%%                                      ����ָ��      �����ݴ�������61~73
%% ����  �洢�������  ����PC��   Matlab��������������Ƿ���һ��
%%  ���� �Ҳ��еģ������е�ͳһ�ſ�װ�뵽 walking.xls��   ������ʽ 23*90
%%                                    dancing.xls
%%                                    sit_dowm.xls
%% Ȼ���ҳ������ʱ ����switch �����ж�      ���������ʲôָ�

%% ����ȫ������ROBOT���ǣ�ͨ���������߱�־  WALKING  ���ͻὫWalk����������ȡ��Ȼ�����ź��� ʱ������������
                                                                                                   
%% ��ʱ����������  ok





