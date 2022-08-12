function varargout = sl_synch_gui_2(varargin)
% SL_SYNCH_GUI_2 M-file for sl_synch_gui_2.fig
%      SL_SYNCH_GUI_2, by itself, creates a new SL_SYNCH_GUI_2 or raises the existing
%      singleton*.
%
%      H = SL_SYNCH_GUI_2 returns the handle to a new SL_SYNCH_GUI_2 or the handle to
%      the existing singleton*.
%
%      SL_SYNCH_GUI_2('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in SL_SYNCH_GUI_2.M with the given input arguments.
%
%      SL_SYNCH_GUI_2('Property','Value',...) creates a new SL_SYNCH_GUI_2 or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before sl_synch_gui_2_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to sl_synch_gui_2_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES
%
% Edited by Will Campbell, MathWorks Inc.
% will.campbell@mathworks.com
% January 7, 2010
% Copyright 2010 The MathWorks, Inc.

% Last Modified by GUIDE v2.5 22-Jan-2019 10:52:24

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @sl_synch_gui_2_OpeningFcn, ...
                   'gui_OutputFcn',  @sl_synch_gui_2_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before sl_synch_gui_2 is made visible.
function sl_synch_gui_2_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure ����ͼ
% eventdata  reserved - to be defined in a future version of MATLAB ������
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to sl_synch_gui_2 (see VARARGIN)�����в���

% Choose default command line output for sl_synch_gui_2  //synch-ͬ��
handles.output = hObject;

% Update handles structure //����
guidata(hObject, handles);

% UIWAIT makes sl_synch_gui_2 wait for user response (see UIRESUME)
% uiwait(handles.figure1);

% Get the value of the manual switch and synch the GUI
%set_param(Object,ParameterName,Value,...ParameterNameN,ValueN)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%         �����sw����ֵ������
value = get_param([bdroot '/Manual Switch'],'sw');
if strcmp(value,'0')  % strcmp---Compare strings
    set(handles.checkbox_switch,'Value',0)                  %  set(H,Name,Value)             
else
    set(handles.checkbox_switch,'Value',1)
end

% Get the value of the gain edit box and synch the GUI
value = get_param([bdroot '/Gain'],'Gain');%��ȡSimulink
set(handles.edit_Gain,'String',value);    %��GUI��ֵ
%���ϸ�������ͨ����ȡsimulink�е�block�ص����
% Set the value of the gain slider, with max/min of +10/-10
slider_position = max(0,min(1,(str2double(value) + 10)/20));
set(handles.slider_Gain,'Value',slider_position);
%%%%%%%%%%%%%%
%��ʼ�����ڲ���
hasData = false;%���������Ƿ���յ�����
isShow = false;  	%�����Ƿ����ڽ���������ʾ�����Ƿ�����ִ�к���dataDisp
strRec = '';%�ѽ��յ��ַ���
numRec = 0;    	%�����ַ�����
numSend = 0;   	%�����ַ�����
%������������ΪӦ�����ݣ����봰�ڶ�����
setappdata(hObject,'hasData',hasData);
setappdata(hObject, 'isShow', isShow);
setappdata(hObject,'strRec',strRec);
setappdata(hObject, 'numRec', numRec);
setappdata(hObject, 'numSend', numSend);

% Assign gui, startstop, switch, and gain handles to the base workspace
assignin('base','sl_synch_handles',handles)
assignin('base','startstop_hObject',handles.pushbutton_startstop)
assignin('base','switch_hObject',handles.checkbox_switch)
assignin('base','gain_hObject',handles.edit_Gain)

%% ��䱳��ͼƬ
ha=axes('units','normalized','position',[0 0 1 1]);
uistack(ha,'down')
II=imread('pic.jpg');
image(II)
colormap winter
set(ha,'handlevisibility','off','visible','off');


% --- Outputs from this function are returned to the command line.
function varargout = sl_synch_gui_2_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in pushbutton_startstop.
function pushbutton_startstop_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_startstop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
%����Simulink����ѡ�ʹ��ʹ�õ�ǰ�����ռ�
%���� Simulink�ӵ�ǰ�����ռ�����
options = simset('SrcWorkspace','current'); %����matlab��simulink������ͬ�Ĺ����ռ�

mystring = get(hObject,'String');
status = get_param(bdroot,'simulationstatus');
if strcmp(mystring,'Start Simulation')
    
    % Check the status of the simulation and start it if it's stopped
    if strcmp(status,'stopped')
        set_param(bdroot,'simulationcommand','start')
    end
    
    % Update the string on the pushbutton
    set(handles.pushbutton_startstop,'String','Stop Simulation')
    
elseif strcmp(mystring,'Stop Simulation')
    
    % Check the status of the simulation and stop it if it's running
    if strcmp(status,'running')
        set_param(bdroot, 'SimulationCommand', 'Stop')
    end
    
    % Update the string on the pushbutton
    set(handles.pushbutton_startstop,'String','Start Simulation')
    
else
    warning('Unrecognized string for pushbutton_startstop') %#ok<WNTAG>
end

% Assign handles and the startstop object to the base workspace
assignin('base','sl_synch_handles',handles)
assignin('base','startstop_hObject',handles.pushbutton_startstop)


% --- Executes on button press in checkbox_switch.
function checkbox_switch_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox_switch (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox_switch

value = get(hObject,'Value');

if value == 0
    set_param([bdroot '/Manual Switch'],'sw','0')
else
    set_param([bdroot '/Manual Switch'],'sw','1')
end

% Assign handles and the switch object to the base workspace
assignin('base','sl_synch_handles',handles)
assignin('base','switch_hObject',handles.checkbox_switch)



function edit_Gain_Callback(hObject, eventdata, handles)
% hObject    handle to edit_Gain (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_Gain as text
%        str2double(get(hObject,'String')) returns contents of edit_Gain as a double

value = get(hObject,'String');

% Update the model's gain value
set_param([bdroot '/Gain'],'Gain',value)

% Set the value of the gain slider, with max/min of +10/-10
slider_position = max(0,min(1,(str2double(value) + 10)/20));
set(handles.slider_Gain,'Value',slider_position);

% Update simulation if the model is running
status = get_param(bdroot,'simulationstatus');
if strcmp(status,'running')
    set_param(bdroot, 'SimulationCommand', 'Update')
end

% --- Executes during object creation, after setting all properties.
function edit_Gain_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_Gain (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on slider movement.
function slider_Gain_Callback(hObject, eventdata, handles)
% hObject    handle to slider_Gain (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider

% Get the value of the gain slider and determine what the gain value should be
slider_position = get(hObject,'Value');
value = num2str(slider_position*20 - 10);

% Update the model's gain value
%% �ر���Ҫ����     set_param();
set_param([bdroot '/Gain'],'Gain',value)
 set_param('sl_synch_2/Gain1','Gain_1',value);
set_param('sl_synch_2/Leftfoot_1','LF_1',value);
set_param('sl_synch_2/Leftfoot_2','LF_2',value);
% Set the value of the gain edit box
set(handles.edit_Gain,'String',value);

% % Update simulation if the model is running
% status = get_param(bdroot,'simulationstatus');
% if strcmp(status,'running')
%     set_param(bdroot, 'SimulationCommand', 'Update')
% end

% --- Executes during object creation, after setting all properties.
function slider_Gain_CreateFcn(hObject, eventdata, handles)
% hObject    handle to slider_Gain (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on key press with focus on checkbox_switch and none of its controls.
function checkbox_switch_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to checkbox_switch (see GCBO)
% eventdata  structure with the following fields (see MATLAB.UI.CONTROL.UICONTROL)
%	Key: name of the key that was pressed, in lower case
%	Character: character interpretation of the key(s) that was pressed
%	Modifier: name(s) of the modifier key(s) (i.e., control, shift) pressed
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in Open_serial.
function Open_serial_Callback(hObject, eventdata, handles)
% hObject    handle to Open_serial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
mystring = get(hObject,'String');
if get(hObject, 'value')
    str_com=get(handles.Serial_interface,'String');
    val_com=get(handles.Serial_interface,'Value');
    str_rate=get(handles.Baud_rate,'String');
    val_rate=get(handles.Baud_rate,'Value');
    switch val_com
        case 1 
        com_n=str_com{1};
        case 2 
        com_n=str_com{2};
        case 3
        com_n=str_com{3};
        case 4
        com_n=str_com{4} ;       
    end
        switch val_rate
        case 1 
        baud_rate=str2num(str_rate{1});
        case 2 
        baud_rate=str2num(str_rate{2});
        case 3
        baud_rate=str2num(str_rate{3});
        end
    set(handles.Open_serial,'String','�رմ���');
    jiaoyan='none';%��У��λ
    data_bits=8;%����λ����
    stop_bits = 1;
    scom=serial(com_n);%�������ڶ���
    %���ô������ԣ�ָ����ص�����
    set(scom, 'BaudRate', baud_rate, 'Parity', jiaoyan, 'DataBits',...
        data_bits, 'StopBits', stop_bits, 'BytesAvailableFcnCount', 10,...
        'BytesAvailableFcnMode', 'byte', 'BytesAvailableFcn', {@bytes, handles},...
        'TimerPeriod', 0.05, 'timerfcn', {@dataDisp, handles});
  
% elseif strcmp(mystring,'�رմ���')  
% %      baud_rate=get(handles.Baud_rate,'Sting')
% %     set(handles.xianshi,'String',baud_rate);
%     set(handles.Open_serial,'String','�򿪴���');
% end  
    %% �����ڶ���ľ����Ϊ�û����ݣ����봰�ڶ���
    set(handles.figure1, 'UserData', scom);
    %% ���Դ򿪴���
    try
        fopen(scom);  %�򿪴���
    catch   % �����ڴ�ʧ�ܣ���ʾ�����ڲ��ɻ�ã���
        msgbox('���ڲ��ɻ�ã�');
        set(hObject, 'value', 0);  %���𱾰�ť 
        return;
    end
   %% �򿪴��ں������ڷ������ݣ���ս�����ʾ������������״ָ̬ʾ�ƣ�
    set(handles.xianshi, 'string', ''); 			%��ս�����ʾ��
    set(hObject, 'String', '�رմ���');  		%���ñ���ť�ı�Ϊ���رմ��ڡ�
else  %���رմ���
    %% ֹͣ��ɾ����ʱ��
    t = timerfind;
    if ~isempty(t)
        stop(t);
        delete(t);
    end    
    %% ֹͣ��ɾ�����ڶ���
    scoms = instrfind;
    stopasync(scoms);
    fclose(scoms);
    delete(scoms);
    set(hObject, 'String', '�򿪴���');  		%���ñ���ť�ı�Ϊ���򿪴��ڡ�
end   

function dataDisp(obj, event, handles)
%	���ڵ�TimerFcn�ص�����
%   ����������ʾ
%% ��ȡ����
hasData = getappdata(handles.figure1, 'hasData'); %�����Ƿ��յ�����
strRec = getappdata(handles.figure1, 'strRec');   %�������ݵ��ַ�����ʽ����ʱ��ʾ������
numRec = getappdata(handles.figure1, 'numRec');   %���ڽ��յ������ݸ���
%% ������û�н��յ����ݣ��ȳ��Խ��մ�������
if ~hasData
    bytes(obj, event, handles);
end
%% �����������ݣ���ʾ��������
if hasData
    %% ��������ʾģ��ӻ�����
    %% ��ִ����ʾ����ģ��ʱ�������ܴ������ݣ�����ִ��BytesAvailableFcn�ص�����
    setappdata(handles.figure1, 'isShow', true); 
    %% ��Ҫ��ʾ���ַ������ȳ���10000�������ʾ��
    if length(strRec) > 10000
        setappdata(handles.figure1, 'strRec', strRec);
    end
    %% ��ʾ����     &&�������ݰ�
    set(handles.xianshi, 'string', strRec);
    %% ���½��ռ���
    set(handles.rec,'string', numRec);
    %% ����hasData��־���������������Ѿ���ʾ
    setappdata(handles.figure1, 'hasData', false);
    %% ��������ʾģ�����
    setappdata(handles.figure1, 'isShow', false);
end    

 
function bytes(obj, ~, handles)
%   ���ڵ�BytesAvailableFcn�ص�����
%   ���ڽ�������
%% ��ȡ����
strRec = getappdata(handles.figure1, 'strRec'); %��ȡ����Ҫ��ʾ������
numRec = getappdata(handles.figure1, 'numRec'); %��ȡ�����ѽ������ݵĸ���
isShow = getappdata(handles.figure1, 'isShow');  %�Ƿ�����ִ����ʾ���ݲ���
%% ������ִ��������ʾ�������ݲ����մ�������
if isShow
    return;
end
%% ��ȡ���ڿɻ�ȡ�����ݸ���
n = get(obj, 'BytesAvailable');
%% �����������ݣ�������������
if n
    %% ����hasData����������������������Ҫ��ʾ
    setappdata(handles.figure1, 'hasData', true);
    %% ��ȡ��������
    a = fread(obj, n, 'uchar');
    %% ��������ΪҪ��ʾ���ַ���
     c = char(a');
    %% �����ѽ��յ����ݸ���
     numRec = numRec + size(a, 1);
    %% ����Ҫ��ʾ���ַ���
     strRec = [strRec c];
    %% ���²���
    setappdata(handles.figure1, 'numRec', numRec); %�����ѽ��յ����ݸ���
    setappdata(handles.figure1, 'strRec', strRec); %����Ҫ��ʾ���ַ���
end













    
    
    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on selection change in Serial_interface.
function Serial_interface_Callback(hObject, eventdata, handles)
% hObject    handle to Serial_interface (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns Serial_interface contents as cell array
%        contents{get(hObject,'Value')} returns selected item from Serial_interface


% --- Executes during object creation, after setting all properties.
function Serial_interface_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Serial_interface (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in Baud_rate.
function Baud_rate_Callback(hObject, eventdata, handles)
% hObject    handle to Baud_rate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
val_baud=get(hObject,'value');
% Hints: contents = cellstr(get(hObject,'String')) returns Baud_rate contents as cell array
%        contents{get(hObject,'Value')} returns selected item from Baud_rate


% --- Executes during object creation, after setting all properties.
function Baud_rate_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Baud_rate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in xianshi.
function xianshi_Callback(hObject, eventdata, handles)
% hObject    handle to xianshi (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns xianshi contents as cell array
%        contents{get(hObject,'Value')} returns selected item from xianshi


% --- Executes during object creation, after setting all properties.
function xianshi_CreateFcn(hObject, eventdata, handles)
% hObject    handle to xianshi (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function rec_Callback(hObject, eventdata, handles)
% hObject    handle to rec (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of rec as text
%        str2double(get(hObject,'String')) returns contents of rec as a double


% --- Executes during object creation, after setting all properties.
function rec_CreateFcn(hObject, eventdata, handles)
% hObject    handle to rec (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function trans_Callback(hObject, eventdata, handles)
% hObject    handle to trans (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of trans as text
%        str2double(get(hObject,'String')) returns contents of trans as a double


% --- Executes during object creation, after setting all properties.
function trans_CreateFcn(hObject, eventdata, handles)
% hObject    handle to trans (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in Sit_down.
function Sit_down_Callback(hObject, eventdata, handles)
% hObject    handle to Sit_down (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
scom = get(handles.figure1, 'UserData');
val = 'Sit_down';
str = get(scom, 'TransferStatus');
if ~(strcmp(str, 'write') || strcmp(str, 'read&write'))
     fwrite(scom, val, 'uint8', 'async'); %����д�봮��
end

% --- Executes on button press in Stand_up.
function Stand_up_Callback(hObject, eventdata, handles)
% hObject    handle to Stand_up (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
scom = get(handles.figure1, 'UserData');
val = 'Stand_up';
str = get(scom, 'TransferStatus');
if ~(strcmp(str, 'write') || strcmp(str, 'read&write'))
     fwrite(scom, val, 'uint8', 'async'); %����д�봮��
end

% --- Executes on button press in Go_up.
function Go_up_Callback(hObject, eventdata, handles)
% hObject    handle to Go_up (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
scom = get(handles.figure1, 'UserData');
val = 'Go_up';
str = get(scom, 'TransferStatus');
if ~(strcmp(str, 'write') || strcmp(str, 'read&write'))
     fwrite(scom, val, 'uint8', 'async'); %����д�봮��
end

% --- Executes on selection change in sends.
function sends_Callback(hObject, eventdata, handles)
% hObject    handle to sends (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns sends contents as cell array
%        contents{get(hObject,'Value')} returns selected item from sends


% --- Executes during object creation, after setting all properties.
function sends_CreateFcn(hObject, eventdata, handles)
% hObject    handle to sends (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in Rest_position.
function Rest_position_Callback(hObject, eventdata, handles)
% hObject    handle to Rest_position (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
scom = get(handles.figure1, 'UserData');
val = 'Rest_position';
str = get(scom, 'TransferStatus');
if ~(strcmp(str, 'write') || strcmp(str, 'read&write'))
     fwrite(scom, val, 'uint8', 'async'); %����д�봮��
end

% --- Executes on button press in Dancing.
function Dancing_Callback(hObject, eventdata, handles)
% hObject    handle to Dancing (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
scom = get(handles.figure1, 'UserData');
val = 'Dancing';
str = get(scom, 'TransferStatus');
if ~(strcmp(str, 'write') || strcmp(str, 'read&write'))
     fwrite(scom, val, 'uint8', 'async'); %����д�봮��
end

% --- Executes on button press in Singing.
function Singing_Callback(hObject, eventdata, handles)
% hObject    handle to Singing (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
scom = get(handles.figure1, 'UserData');
val = 'Singing';
str = get(scom, 'TransferStatus');
if ~(strcmp(str, 'write') || strcmp(str, 'read&write'))
     fwrite(scom, val, 'uint8', 'async'); %����д�봮��
end

% --- Executes on button press in Stop.
function Stop_Callback(hObject, eventdata, handles)
% hObject    handle to Stop (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
scom = get(handles.figure1, 'UserData');
val = 'Stop';
str = get(scom, 'TransferStatus');
if ~(strcmp(str, 'write') || strcmp(str, 'read&write'))
     fwrite(scom, val, 'uint8', 'async'); %����д�봮��
end
