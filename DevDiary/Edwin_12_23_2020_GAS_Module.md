Flushing, 3:00 AM

I've went back and forth today on whether to include the Gameplay Ability System shooter code as coupled code with the current shooter game code, but decided to decouple it somewhat as a module.

If I really wanted the GAS shooter to be used in different projects, a plugin would be better than a module. Since I'm planning on coupling the game and GAS code somewhat, a module represents the best solution at this time. Turning a module into a plugin is not difficult anyway.

Moreover, it's a good idea to keep the complex and suspect-to-change Gameplay Ability System code away from the main gameplay logic.

Pro-tip: If copying over files and creating new files, to see them pop in Visual Studio, have to right click on unreal exe and "Generate Project Files"

Fix these logging issues:
Macro redefinitions for COLLISION_PICKUP and COLLISION_PROJECTILE

[2020.12.25-02.39.19:031][  0]LogConsoleManager: Warning: Console object named 'replay.MontageErrorThreshold' already exists but is being registered again, but we weren't expected it to be! (FConsoleManager::AddConsoleObject)
'UE4Editor-Win64-DebugGame.exe' (Win32): Loaded 'C:\Users\edwin\Documents\Unreal Projects\FPSGame\Binaries\Win64\UE4Editor-ShooterGame-Win64-DebugGame.dll'. Symbols loaded.
[2020.12.25-02.39.19:200][  0]LogLinker: Warning: Failed to load '/Game/GASShooter/Characters/Hero/BP_HeroCharacter': Can't find file.
[2020.12.25-02.39.19:200][  0]LogLinker: Warning: Failed to load '/Game/GASShooter/Characters/Hero/BP_HeroCharacter': Can't find file.
[2020.12.25-02.39.19:200][  0]LogUObjectGlobals: Warning: Failed to find object 'Class /Game/GASShooter/Characters/Hero/BP_HeroCharacter.BP_HeroCharacter_C'
[2020.12.25-02.39.19:201][  0]LogTemp: Error: AGASShooterGameModeBase::AGASShooterGameModeBase() Failed to find HeroClass. If it was moved, please update the reference location in C++.
[2020.12.25-02.39.19:201][  0]LogLinker: Warning: Failed to load '/Game/GASShooter/UI/WC_DamageText': Can't find file.
[2020.12.25-02.39.19:201][  0]LogLinker: Warning: Failed to load '/Game/GASShooter/UI/WC_DamageText': Can't find file.
[2020.12.25-02.39.19:202][  0]LogUObjectGlobals: Warning: Failed to find object 'Class /Game/GASShooter/UI/WC_DamageText.WC_DamageText_C'
[2020.12.25-02.39.19:202][  0]LogTemp: Error: AGSCharacterBase::AGSCharacterBase() Failed to find DamageNumberClass. If it was moved, please update the reference location in C++.
[2020.12.25-02.39.19:202][  0]LogTemp: Error: AGSCharacterBase::AGSCharacterBase() Failed to find DamageNumberClass. If it was moved, please update the reference location in C++.
[2020.12.25-02.39.19:203][  0]LogLinker: Warning: Failed to load '/Game/GASShooter/UI/UI_FloatingStatusBar_Hero': Can't find file.
[2020.12.25-02.39.19:203][  0]LogLinker: Warning: Failed to load '/Game/GASShooter/UI/UI_FloatingStatusBar_Hero': Can't find file.
[2020.12.25-02.39.19:203][  0]LogUObjectGlobals: Warning: Failed to find object 'Class /Game/GASShooter/UI/UI_FloatingStatusBar_Hero.UI_FloatingStatusBar_Hero_C'
[2020.12.25-02.39.19:203][  0]LogTemp: Error: AGSHeroCharacter::AGSHeroCharacter() Failed to find UIFloatingStatusBarClass. If it was moved, please update the reference location in C++.

[2020.12.25-02.34.54:597][  0]LogClass: Warning: Property ShooterBarricade::Health (SourceClass: ShooterBarricade) was not registered in GetLifetimeReplicatedProps. This property will not be replicated. Use DISABLE_REPLICATED_PROPERTY if not replicating was intentional.
[2020.12.25-02.34.54:598][  0]LogClass: Warning: Property ShooterBarricade::LastTakeHitInfo (SourceClass: ShooterBarricade) was not registered in GetLifetimeReplicatedProps. This property will not be replicated. Use DISABLE_REPLICATED_PROPERTY if not replicating was intentional.

Good tutorials
https://docs.unrealengine.com/en-US/ProgrammingAndScripting/GameplayArchitecture/Gameplay/index.html
http://kantandev.com/articles/ue4-code-modules
https://github.com/kamrann/KantanCodeExamples
https://github.com/kamrann/KantanCharts
https://answers.unrealengine.com/questions/667773/how-to-design-a-game-using-modules-and-ue4-design.html
https://sandordaemen.nl/blog/creating-an-editor-module-in-unreal-engine-4/
https://docs.unrealengine.com/en-US/ProductionPipelines/BuildTools/UnrealBuildTool/ModuleFiles/index.html
https://github.com/DominikPeacock/ue4-new-module-tool
http://cairansteverink.nl/cairansteverink/blog/writing-a-custom-asset-editor-for-unreal-engine-4-part-1/
http://cairansteverink.nl/cairansteverink/blog/creating-an-editor-module-in-unreal-engine-4/
https://www.ue4community.wiki/creating-cpp-module-oshdsg2t
https://answers.unrealengine.com/questions/23384/what-is-the-difference-between-publicdependencymod.html
https://answers.unrealengine.com/questions/20310/explanation-of-source-code-folder-structure.html
https://answers.unrealengine.com/questions/942407/view.html?sort=oldest