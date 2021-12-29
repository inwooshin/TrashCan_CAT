# Trash Can   


## 목차    
[1. 개요](#개요)   
[2. 발표영상](#발표영상)   
[3. 사용 CPU 및 센서](#사용-cpu-및-센서)   
[4. 구조 설계](#구조-설계)   
[5, 사용 STL 도면](#사용-stl-도면)   
[6. 외형](#외형)   
[7. 시연 영상](#시연-영상)   
[8. 조원별 역할](#조원별-역할)   

   
## 개요

![push_pull](https://user-images.githubusercontent.com/59462895/143794693-9f2fb1c4-2580-4ded-b696-f62d02f65877.gif)

이 프로젝트는 압축 기능 및 IoT 기능을 사용할 수 있는 쓰레기통인 C.A.T(Can do Anything Trash can) 에 관한 프로젝트이다. WiFi 라이브러리를 사용하고 HTTP 로 ESP8266 과 앱인벤터의 통신이 가능하도록 설계하였다. 그리고 STL 파일으로 Press Hardware 를 수정하여 Scissors Linkage 의 구조로 3D 프린팅 하였으며, Torque 가 부족하기 때문에 기존의 gear 를 1개에서 2개로 사용을 변경하여 Push 와 Pull 이 용이하게 만들었다.
   
## 발표영상
[![발표영상화면](https://user-images.githubusercontent.com/59462895/143811073-9cc129e4-b6d4-4e46-b6c8-dc1d9ab2548a.JPG)](https://www.youtube.com/watch?v=RVP99xg6Qa0)   
위의 사진을 클릭하시면 Youtube 링크로 이동합니다.   

## 사용 CPU 및 센서

- **ESP8266**
    
    ![esp](https://user-images.githubusercontent.com/59462895/143794727-13302c5f-6ff4-4467-9ca8-960c9f6609ae.jpg)
    
    1. 전체 GPIO 핀을 제어하기 위한 메인보드
    2. Application과 연결하기 위한 WiFi 모듈 내장
    3. 3.3V Power Logic
- **12V Motor Driver**
    
    ![MotorDriver](https://user-images.githubusercontent.com/59462895/143794728-552d1dd4-9752-4789-99cf-d587730d5a5f.jpg)
    
    1. PWM을 통해서 모터속도 제어 가능
    2. 정전압 역전압으로 양방향 제어 가능
    3. 입력 전압 : 5 ~ 35 V / 동작 전압 : 5V
- **Pololu 50:1 Metal Gearmotor 12V**
    
    ![gear](https://user-images.githubusercontent.com/59462895/143794847-cf6b1843-c8ae-4fac-874a-6e08ff8f9eab.jpg)
)
    
    1. Rack - Pinion 을 구동하기 위한 DC Motor
    2. 구동전압 : 12V / RPM : 200 / Stall Torque : 21kg*cm
    / 정격 설계 : 4.2kg*cm
- **MP3 Module & Speaker**
    
    ![speaker](https://user-images.githubusercontent.com/59462895/143794706-72e81cfd-2b8c-485f-b6be-9c0e6cfd053e.jpg)
    
    1. 3W 전력 / 4Ω Impedance / 직경 : 50mm
    2. 최대 255개의 파일 저장 가능 / 30 Volume Level / 24bit DAC Output
- **DC-DC Convertor**
    
    ![conv](https://user-images.githubusercontent.com/59462895/143794726-61e971b5-f441-45f1-ba7a-69a3111e5702.jpg)
    
    1. 전원의 12V를 Node MCU 5V 정격 전압으로 강하
    2. 입력 전압 7V ~ 26V / 출력 전압 : 5V / 출력 전류 : 최대 3A
    변환 효율 : 최대 96%
- **단면 PCB 기판**
    
    ![pcb](https://user-images.githubusercontent.com/59462895/143794732-2ca28eb8-5f0e-4106-ae50-35299b156fb7.jpg)
    
    1. 브레드 보드를 대체하여 사용
    2. 메인보드 및 각 센서를 납땜하여 사용
- **아크릴 판**
    
    ![아크릴](https://user-images.githubusercontent.com/59462895/143794712-b4622b5e-654c-458d-8fc9-12276e54b104.jpg)
    
    1. 본 프로젝트의 외형을 구현, 도면에 따라 재단해서 사용
    - 강판 두께 : 3T(3mm) / 강판 크기 : 500(mm)x500(mm)
- **SR - 04 초음파 센서**
    
    ![cho](https://user-images.githubusercontent.com/59462895/143794959-6940f505-68ee-4853-ba55-b9010e015653.jpg)
)
    
    1. 동작전압 : 5V / 동작전류 : 15mA / 동작 주파수 : 40Hz
    발생 주파수 : 40kHz / 측정거리 : 2~400cm / 감지각도 : 15
    크기 : 45 x 20 x 15(mm)
- **12V 건전지**
    
    ![12v](https://user-images.githubusercontent.com/59462895/143794723-c0dcce56-cd4e-4d30-9317-0fbc78fa2e82.jpg)
    
    1. 8개입 건전지 CASE를 이용해 [1.5V x 8] 개 12V 전압 입력
- **SG-90**
    
    ![servo](https://user-images.githubusercontent.com/59462895/143794705-e49e1386-5c24-4f01-b9e8-b46b003e26fd.jpg)
    
    1. 쓰레기 투입구를 자동으로 열기 위한 Motor
    2. 각도를 0 ~ 180로 변화시키면서 연결된 Wire를 통해 문을 개폐
    3. - Weight : 9g / Torque : 1.3kg*cm(at 4.8V) / RPM : 110

### 구조 설계

- **Hardware**
    - **Scissors Linkage**
        
        ![ScissorsMechan](https://user-images.githubusercontent.com/59462895/143794704-3c739a30-45c8-432e-af29-21a994e222b5.JPG)
        
        구조는 Scissors Mechanism 의 구조로 설계하였다. 위에 부착해서 프레스를 해줘야하는 구조상 크기를 최대한 키우지 않을 수 있게 다리를 안으로 접을 수 있는 구조인 Scissors Mechanism 을 선택했다. 
        
    - **외형 설계**
        
        ![외형설계](https://user-images.githubusercontent.com/59462895/143794717-60e8e2a3-43a5-4b19-b515-62b05b0c0c7f.png)
        
    - **회로 설계**
        
        ![회로설계](https://user-images.githubusercontent.com/59462895/143794722-797fa7ac-e2ed-4c9d-ac04-beb3d61e407a.jpg)
        
- **Software**
    - ESP8266 Code (C++)
        
        12V 의 전원을 인가하게 되면 5V 로 변경시켜주는 레귤레이터를 통해서 ESP 8266 이 동작하게 된다. 그 ESP8266 에 달린 입구 부분의 초음파 센서를 확인해서 인접한 거리에 사람이 20Cm 내에 있는지 확인한다. 그리고 내에 있을 경우 입구를 오픈합니다. 또한 쓰레기의 잔량을 확인할 수 있도록 압축 판에 부착한 초음파센서가 쓰레기의 잔량을 재서 20Cm 보다 작으면 압축 동작을 시킬 수 있도록 하였다. 그리고 앱인벤터의 신호를 확인해서 들어오는 Open 이나 Press 그리고 Remain check 같은 잔량 체크를 하는 동작을 수행할 수 있도록 하였다.
        
        ![소프트웨어플로우차트](https://user-images.githubusercontent.com/59462895/143794711-5d9d64db-eeb8-416a-a74a-b47163ead4e9.JPG)
        
    - App Inventor
        
        우선 HTTP 의 GET 을 통해서 Compression 의 PRESS 라는 키워드를 넣어서 접속할 경우 해당 키워드에 접속을 주시하고 있던 ESP8266 이 Press 동작을 수행한다. 그리고 이 동작을 한 후 응답을 보내 온전하게 연결이 되고 동작을 수행했는지를 알 수 있도록 하였다. 이 형식으로 Remain check, 딜레이 시간을 조정하는 push 와 pull 의 딜레이 시간을 조정할 수 있도록 하였다.
        
        ![앱인벤터플로우차트](https://user-images.githubusercontent.com/59462895/143794716-c478d094-5744-4c65-99dc-42447f34f4cb.JPG)
        

### 사용 STL 도면

- Linkage 상하단 구조물

![stl-1](https://user-images.githubusercontent.com/59462895/143794707-dca707f8-10b0-40e8-a2cb-ffb21db02f3f.JPG)

[https://www.thingiverse.com/thing:3483911](https://www.thingiverse.com/thing:3483911) 의 Scissors Mechanism 도면을 사용하였다. 기존의 것과 다른 것은 아래의 Second Gear 구조물의 Pinion Gear 와 Linkage 상단의 Rack Gear 의 align 을 맞출 수 있도록 수정하였다.

- Second Gear 구조물

![stl-2](https://user-images.githubusercontent.com/59462895/143795215-9bbc82b1-f187-4e9b-99d1-d69be0760d4a.JPG)


[https://www.thingiverse.com/thing:677144](https://www.thingiverse.com/thing:677144) 의 도면을 수정하여 사용하였다. 원도면과 Second Gear Holder 가 굉장히 다른 모양이다. 이것은 Linkage 의 도면과 부착하는 부분이 없기 때문에 고정해줄 수 있는 부분을 만들어서 부착해주었고 두번째 기어를 고정하는 부분을 위로 Shift 시켜서 수정했기 때문이다. 그리고 부족한 토크로인한 기어비를 3:1 로 최대로 주기 위해서 조금 더 수정하고 기어와의 align 을 수정하였다.

## 외형

- 정면
    
    ![전면](https://user-images.githubusercontent.com/59462895/143794719-605f2f6a-a085-4e82-ba75-72ad6687930a.jpg)
    
- 후면
    
    ![후면](https://user-images.githubusercontent.com/59462895/143795263-2baeac30-e5ad-48dc-8652-8fa702417060.jpg)
    

- 측면
    
    ![측면](https://user-images.githubusercontent.com/59462895/143795260-3a5add25-bb56-4d19-be5b-a98a11fad0b6.jpg)
    

## 시연 영상

- Push n Pull

![push_pull](https://user-images.githubusercontent.com/59462895/143794693-9f2fb1c4-2580-4ded-b696-f62d02f65877.gif)   
- 앱인벤터로 Press n Pull

![앱인벤터푸시](https://user-images.githubusercontent.com/59462895/143794714-99e06f46-c3f2-4fba-b644-7faccb6c7fab.gif)

- 외부 사용자 감지 시

![open](https://user-images.githubusercontent.com/59462895/143794730-9f5756a1-1cee-48b5-a6f2-e96c4ad5fdca.gif)

- 쓰레기통 남은량 감지
    - 전부 비어있을 때
    
    ![remainCheck_full](https://user-images.githubusercontent.com/59462895/143794698-03985ac2-54ef-41ab-8f7d-3362e4493884.gif)
    
    - 조금만 차있을 때
    
    ![remainCheck_half](https://user-images.githubusercontent.com/59462895/143794702-90530c7a-8928-4d3b-8f6e-500cf39ab04b.gif)
    

## 조원별 역할

- 신인우(조장)
    - 전체 코드 구현
    - 하드웨어 설계
    - 부품 선정
    - STL 도면 수정
- 김홍민
    - 하드웨어 설계
    - 부품 선정
    - 외형 설계
    - 음성파일 제작
- 유건환
    - 외형 설계
    - PCB 납땜
    - 외형에 하드웨어 부착
    - 앱인벤터 제작
