// Copyright CrossWoods Consulting

#include "SprungWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	//this->SetTickGroup(TG_PostPhysics);

	MassWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassWheelConstraint"));
	SetRootComponent(MassWheelConstraint);

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Axle->SetupAttachment(MassWheelConstraint);

	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->SetupAttachment(Axle);

	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstraint"));
	AxleWheelConstraint->SetupAttachment(Axle);

	//PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.bTickEvenWhenPaused = true;
	//PrimaryComponentTick.TickGroup = TG_PostPhysics;

}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();

	Wheel->SetNotifyRigidBodyCollision(true);
	Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);

	SetupConstraint();
}

void ASprungWheel::SetupConstraint()
{
	if (!GetAttachParentActor()) { return; }
	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());
	if (!BodyRoot) { return; }
	MassWheelConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Axle, NAME_None);
	AxleWheelConstraint->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("current tick group: %s"), *GETENUMSTRING("ETickingGroup", GetWorld()->TickGroup));

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		//UE_LOG(LogTemp, Warning, TEXT("settting force back to 0 %f"), GetWorld()->GetTimeSeconds());
		TotalForceMagnitudeThisFrame = 0;
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("current tick group: %s"), *GETENUMSTRING("ETickingGroup", GetWorld()->TickGroup));
	}
}

void ASprungWheel::AddDrivingForce(float ForceMagnitude)
{
	TotalForceMagnitudeThisFrame += ForceMagnitude;
}

void ASprungWheel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnHit %f"), GetWorld()->GetTimeSeconds());
	ApplyForce();
}

void ASprungWheel::ApplyForce()
{
	//UE_LOG(LogTemp, Warning, TEXT("adding force %f : %f"), TotalForceMagnitudeThisFrame, GetWorld()->GetTimeSeconds());
	Wheel->AddForce(Axle->GetForwardVector() * TotalForceMagnitudeThisFrame);
}