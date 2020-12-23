Flushing, 3:00 AM

I've went back and forth today on whether to include the Gameplay Ability System shooter code as coupled code with the current shooter game code, but decided to decouple it somewhat as a module.

If I really wanted the GAS shooter to be used in different projects, a plugin would be better than a module. Since I'm planning on coupling the game and GAS code somewhat, a module represents the best solution at this time. Turning a module into a plugin is not difficult anyway.

Moreover, it's a good idea to keep the complex and suspect-to-change Gameplay Ability System code away from the main gameplay logic.

Pro-tip: If copying over files and creating new files, to see them pop in Visual Studio, have to right click on unreal exe and "Generate Project Files"

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