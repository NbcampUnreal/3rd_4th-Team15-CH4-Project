# 내 알을 낳아도(Oh My Egg)

Uneral Engine5로 개발한 **데디케이티드 서버** 기반 **멀티플레이 게임**입니다.  

<img width="1083" height="610" alt="image" src="https://github.com/user-attachments/assets/bea49162-2b6b-4ccd-a285-7bda3a3bd7e8" />

---

## 📑 목차 (Table of Contents)  
1. [개요 (Project Outline)](#-개요-project-outline)
2. [개발 환경 (Development Environment)](#-개발-환경-development-environment)
3. [시연 영상 (Demo Video)](#-시연-영상-demo-video)  
4. [설치 및 실행 방법 (Installation & Run)](#-설치-및-실행방법-Installation--Run)
5. [팀원 및 역할 (Team & Roles)](#-팀원-및-역할-Team--Roles)
6. [기술적 세부사항 (Details)](#-기술적-세부사항-Details)
7. [트러블슈팅 (Troubleshooting)](#-트러블슈팅-Troubleshooting)
8. [소감 및 회고 (Review)](#-소감-및-회고-Review)
9. [Source](#-Source)

---

## 🎮 개요 (Project Outline)  
> **게임 이름** : 내 알을 낳아도 (Oh My Egg) <br>
> **게임 목표** : 수 많은 AI 사이에서, 다른 플레이어 몰래 가장 많은 알을 남긴 닭이 승리한다. <br>
> **주요 특징** : 멀티플레이, 시퀀스, GAS, Seamless Tavel, Dedicated Server, AWS <br>

### Game Flow
<img width="1453" height="705" alt="image" src="https://github.com/user-attachments/assets/88c714ac-f27e-4605-b725-13b1af39444e" />

### WBS (Work Breakdown Structure)
<img width="1547" height="743" alt="image" src="https://github.com/user-attachments/assets/900f35bd-2449-469c-897f-f29470785094" />

---

## 🛠 개발 환경 (Development Environment)  

> **게임엔진** : Unreal Engine 5.5.4 <br>
> **개발언어** : C++ / Blueprint <br>
> **타겟 플랫폼** : Windows <br>
> **협업 툴** : GitHub LFS / [Notion](https://www.notion.so/teamsparta/15-2552dc3ef51480e9bfa2de85aca7674e) / Miro / Figma / Slack / Google Drive / Canva / Zep <br>

---

## 📽 시연 영상 (Demo Video)  

### **Description vedeio** <br>
  [![Demo Vedio](https://img.youtube.com/vi/cqkaqhopTks/0.jpg)](https://www.youtube.com/watch?v=cqkaqhopTks)

### **Summary InGame Play** <br>
  [![Ending Cinematic](https://img.youtube.com/vi/QGKolVZmCcQ/0.jpg)](https://www.youtube.com/watch?v=QGKolVZmCcQ)
  
### **Full InGame Play** <br>
[![Presentation Video](https://img.youtube.com/vi/rk6yE7ONKY8/0.jpg)](https://www.youtube.com/watch?v=rk6yE7ONKY8)

### [발표자료](https://www.canva.com/design/DAG1usVWwt4/l-QsvJdKDZC0Gl3QCC2TzQ/view?utm_content=DAG1usVWwt4&utm_campaign=designshare&utm_medium=link2&utm_source=uniquelinks&utlId=h5a7c291f5a)

---

## 🏗 설치 및 실행 방법 (Installation & Run)  

* **[클라이언트 프로그램 다운로드](https://drive.google.com/file/d/1sji0FaB2veCAXj5Ci_4m7v0IMi40iq41/view?usp=sharing)**
* **[서버 프로그램 다운로드](https://drive.google.com/file/d/1ju1qZCX6TXqyaDwOvDthkG6MbkB38Z8Z/view?usp=sharing)**

<img width="256" height="256" alt="ohmyegg" src="https://github.com/user-attachments/assets/8f51fbda-53d7-42a9-a989-934c44e6b40a" />


---

## 👨‍💻 팀원 및 역할 (Team & Roles)  

| 이름   | 역할 (Role)       | 담당 파트 | Blog | Github
|--------|------------------|-----------|-------|-------
| 고명수 | Game Framework, Server | 게임 규칙 및 진행 | [Velog](https://velog.io/@vvmu13/posts) | [Github](https://github.com/BannedUser0)
| 김세훈 | Player, GAS | 플레이어 캐릭터, GAS | [Velog](https://velog.io/@sammy3831/posts) | [Github](https://github.com/sammy3831) |
| 김  혁 | AI, GAS | AI 캐릭터, GAS | [Velog](https://velog.io/@uji06134) | [Github](https://github.com/americanotwoshot) |
| 김효영 | Game Framework, Server | 서버관리, 패키징 | [NaverBlog](https://blog.naver.com/comekgr1564) | [Github](https://github.com/khy1564) |
| 장주만 | GAS, VoiceChat | GAS, 보이스 채팅 | [NaverBlog](https://blog.naver.com/letterb926) | [Github](https://github.com/JangJuMan/) |
| 한국인 | UI, Level Design | UI, 레벨 디자인 | [Tistory](https://han-ha.tistory.com/manage) | [Github](https://github.com/rnrls) |

---

## 🏗 기술적 세부사항 (Details)

### Gameplay Ability System (GAS)

<img width="1577" height="664" alt="image" src="https://github.com/user-attachments/assets/c97f7584-f675-41ef-afbd-beddd87389e0" />

> Ability System Component : 시스템과의 모든 상호작용을 처리하는 컴포넌트 <br>
> Attribute Set : 액터에 속한 게임 플레이 관련 수치 값 <br>
> Gameplay Ability : 액터가 할 수 있는 모든 액션, 스킬 <br>
> Gameplay Effect : 액터의 Attribute Set의 값이나 Gameplay Tag에 영향을 주는 효과 <br>
> Gameplay Cue : 사운드나 파티클 같은 비 게임플레이 관련 작업을 하는 큐 <br>

### EOS(Epic Online Service) Voice Chat
> Epic Games에서 제공하는 크로스 플랫폼 온라인 서비스 중, Voice Chat 기능을 사용 <br>
> P2Server 구조로 Client ↔ Voice Server 간 RTC(Real-Time Communication) 연결 <br>

### Using Subsystem
> Subsystem : 언리얼 엔진이 제공하는 글로벌 매니저형 객체 시스템 <br>
> 특정, 라이프 사이클(Game Instnace, World 등0에 맞춰 자동 생성 및 관리 <br>
> 적용 분야 : BGM, SFX Manager(← UGameInstanceSubsystem), Egg Pool Manager(← UWorldSubsystem) <br>

### Seamless Travel
> 멀티플레이에서 맵(레벨)을 전환할 때 클라이언트를 재접속시키지 않고, 서버와의 연결을 유지한 채로 새로운 맵을 불러오는 기능 <br>
> 플레이어 관련 주요 객체(PlayerController, PlayerState)가 세션을 유지한 채로 이동(로딩 중에도 네트워크 연결이 유지됨) <br>

### Door & Dynamic Navmesh
> 멀티플레이 환경에서 문 열기 + AI의 Nav Mesh를 동적으로 연동 <br>
> 서버-클라이언트 간 문 상태 동기화 <br>
> <img width="645" height="260" alt="image" src="https://github.com/user-attachments/assets/ef754865-fc0a-4229-9f50-ae92f079db34" />

---

## 🚑 트러블슈팅 (Troubleshooting)  

### 1. Seamless Travel
- **문제 상황**  
  - 기존 생명주기 함수에서 실행하던 방장 재 지정 함수가 호출되지 않음 (Pre/Post Login)
- **문제 원인**  
  - `Seamless Travel` 이 활성화되면 플레이어 컨트롤러가 파괴되지 않기 때문에 Pre/Post Login 이 호출되지 않음
- **해결 방법**  
  - Seamless Travel 상황의 생명주기 함수인 HandleSeamlessTravelPlayer(AController*& C) 를 활용

### 2. Seamless Travel + Sequence
<img width="1638" height="686" alt="image" src="https://github.com/user-attachments/assets/ae2db683-842b-486d-97a4-967d6b8ffd1e" />

### 3. Server-Client 동기화 (GAS)
<img width="1635" height="678" alt="image" src="https://github.com/user-attachments/assets/e85bded8-0258-4fae-8dc8-ef1c46062f5f" />

### 4. Dangling Pointer
- **문제 상황/원인**  
  - 전역적으로 SFX Manager에 접근하고, 사운드를 재생하기 위해 UGameInstanceSubsystem을 상속받았고, 내부의 UAudioComponent의 outer를 GameInstance Subsystem으로 설정함.
  - 그러나 해당 AudioComponent를 World에 등록했기 때문에 포인터와 객체의 수명이 불일치하는 문제가 발생함
- **실행 결과**  
  - 레벨이동 시 월드는 파괴되지만, UAudioComponent의 Outer(= GameInstance)는 여전히 살아있기 때문에 GC 대상에서 제외됨
  - 따라서 SFX 재생 시 Dangling Pointer 참조로 Crash 발생
  <img width="1426" height="202" alt="image" src="https://github.com/user-attachments/assets/824e26d9-1412-4a1c-a5d0-a88f528f4290" />
- **해결 방법**  
  - Outer를 WorldSettings로 설정하여, World와 생명주기를 일치시킴
  - World가 파괴될 때 UAudioComponent도 같이 파괴되도록 하여 Dangling Pointer가 발생하지 않도록 함
  <img width="1405" height="441" alt="image" src="https://github.com/user-attachments/assets/20898544-1598-4dbb-94ae-2a8e78f4c2f2" />

---

## 🚑 소감 및 회고 (Review)
<img width="1598" height="655" alt="image" src="https://github.com/user-attachments/assets/bac87c57-0d40-4df1-8f2c-0ae2af131908" />


---

