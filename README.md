# ProjectUSA - Stylish Action Game

언리얼 엔진 5.2 기반의 스타일리시 액션 게임 프로젝트입니다. (싱글플레이 / 코옵 지원)

## 시연 영상

[![시연 영상](https://img.youtube.com/vi/WplH1UWZAgM/0.jpg)](https://youtu.be/WplH1UWZAgM)

## 개요

ProjectUSA는 **마리오**의 플랫포머 요소와 **데빌 메이 크라이**의 스타일리시 액션을 결합한 게임입니다. 화려한 액션 전투 시스템, 캐릭터 진행, 레벨 기반 게임플레이를 특징으로 하며, 싱글플레이와 코옵 플레이를 지원합니다.

**버전**: 0.1.10+
**엔진**: Unreal Engine 5.2
**플랫폼**: Windows 64-bit

## 주요 기능

### 전투 시스템
- **공격 추적 시스템**: 직접 타겟팅, 오프셋 추적, 단일/다중 히트 지원
- **무기 시스템**: 장착 가능한 무기, 소켓 부착, 무기별 어빌리티 연동
- **피격 효과**: Niagara 파티클 VFX, 젤리 이펙트 시각 효과

### Gameplay Ability System (GAS)
- 시작/트리거/대미지 반응/사망/승리/리스폰 어빌리티
- 20개 이상의 커스텀 어빌리티 태스크
- 모듈식 캐릭터 액션 시스템

### 캐릭터 시스템
- **플레이어 캐릭터**: Enhanced Input, 이중 카메라 (자유 시점 + 타겟팅), 카메라 쉐이크
- **NPC**: AI 행동 트리, 자동 타겟팅

### 아이템 시스템
- 픽업/사용 애니메이션
- 인벤토리 관리
- 아이템별 게임플레이 효과

### 카메라 시스템
- 타겟팅 카메라 (전투 락온)
- 배치 카메라 (연출)
- 부드러운 카메라 전환

### 코옵 플레이
- 무기, 아이템, 캐릭터 상태 동기화
- 서버/클라이언트 RPC 구조
- 네트워크 역할 검증

## 프로젝트 구조

```
Source/ProjectUSA/
├── Character/           # 캐릭터 클래스 및 컴포넌트
├── GAS/                 # Gameplay Ability System
│   ├── GA/              # Gameplay Abilities
│   ├── GE/              # Gameplay Effects
│   ├── AT/              # Ability Tasks
│   └── AttributeSet/    # 어트리뷰트 정의
├── Item/                # 아이템 시스템
├── Weapon/              # 무기 시스템
├── Camera/              # 카메라 액터
├── Spawner/             # 스포너 시스템
├── Interface/           # 인터페이스 정의
├── AnimNotify/          # 애니메이션 노티파이
└── Data/                # 데이터 에셋

Content/ProjectUSA/
├── AI/                  # AI 블루프린트
├── Animations/          # 애니메이션
├── Blueprints/          # 블루프린트
│   ├── Characters/
│   ├── Gamemodes/
│   ├── Items/
│   └── Weapons/
├── DamageTypes/         # 대미지 타입
├── Maps/                # 레벨
│   ├── Demo_MainTitle/  # 타이틀 화면
│   └── Demo_MainGame/   # 메인 게임플레이
├── Materials/           # 머티리얼
├── Models/              # 3D 모델
├── Music/               # 배경음악
├── Sounds/              # 효과음
├── UI/                  # UI 위젯
└── VisualEffects/       # Niagara 파티클
```

## 핵심 클래스

| 클래스 | 설명 |
|--------|------|
| `USACharacterBase` | 기본 캐릭터 (AbilitySystem, Targeting, Damage 지원) |
| `USACharacterPlayer` | 플레이어 캐릭터 (Enhanced Input, 카메라 시스템) |
| `USACharacterNonPlayer` | NPC 캐릭터 (AI 행동 트리) |
| `USACharacterAttackComponent` | 공격 추적 및 대미지 처리 |
| `USAWeaponBase` | 무기 베이스 클래스 |
| `USAItemBase` | 아이템 베이스 클래스 |
| `USATargetingCameraActor` | 전투 락온 카메라 |

## 의존성

- GameplayAbilities
- AIModule
- EnhancedInput
- Niagara
- LevelSequence & MovieScene
- UMG (UI)

## 빌드

1. Unreal Engine 5.2 설치
2. 프로젝트 열기: `ProjectUSA.uproject`
3. Visual Studio 솔루션 생성 후 빌드

## 게임 흐름

1. **타이틀 화면** - 게임 시작 메뉴
2. **메인 게임** - 플레이어 vs AI 전투
3. **리스폰** - 사망 후 부활
4. **승리** - 레벨 클리어

## 최근 업데이트

- 캐릭터 액션/인터페이스 코드 수정
- 사운드 디자인 구현
- 레벨 디자인 및 진행
- AI 패스파인딩 (RVO 설정)
- Niagara 피격 효과 분리
- 젤리 변형 효과 시스템
- 튜토리얼 구현
- 아이템 시스템 및 인벤토리
- 승리/리스폰 메카닉
- 코옵 팀킬 방지
- 타이틀 화면 구현
- 카메라 블렌딩 시스템

