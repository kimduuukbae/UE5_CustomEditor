# UE5_CustomEditor

### 1. Advance Deletion

![5](https://github.com/kimduuukbae/UE5_CustomEditor/assets/39338850/ad27c4dd-6576-4e4b-9e1f-fec637397c3c)

You can check the reference count of the assets.

SearchBox lets you find an asset with a name.

ComboBox lets you check the list of unused assets.

Within this tab, you can delete any asset you want.


### 2. Quick Material Create

![6](https://github.com/kimduuukbae/UE5_CustomEditor/assets/39338850/f6aa0e41-79b6-4070-94dc-bb721f8b95c2)

![Honeycam 2024-01-23 21-16-39](https://github.com/kimduuukbae/UE5_CustomEditor/assets/39338850/3a3c33ff-ab1d-44b4-8ca4-8d27bf93a392)

You can choose the texture you want and quickly create the material.

All Textures are automatically connected to Pin as long as the name conventions are followed.

### 3. Level Actor Locking

![7](https://github.com/kimduuukbae/UE5_CustomEditor/assets/39338850/ce299b29-cbf1-4177-89fc-acb51348897a)

You can lock an Actor that exists in the Level.

Locked Actor cannot be select(Cannot be Click).

This function has modified the Engine Code.

![8](https://github.com/kimduuukbae/UE5_CustomEditor/assets/39338850/22166567-eadf-49f9-a7b4-fba4411890fb)

Added variable "bIsLocked" from "Actor.h"

![9](https://github.com/kimduuukbae/UE5_CustomEditor/assets/39338850/630628aa-4b5c-4ea4-8926-dadbe8f55aa5)

Changed the WorldOutliner Text color in "ActorTreeItem.cpp"

### 4. Impl...
