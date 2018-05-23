#pragma once

UENUM(BlueprintType)

enum class EItems : uint8
{
	none UMETA(Displayname = "none"),
	i01_Potion_Heal50 UMETA(Displayname = "i01_Potion_Heal50"),
	i02_Potion_Heal100 UMETA(Displayname = "i02_Potion_Heal100"),
	i03_Potion_Poison UMETA(Displayname = "i03_Potion_Poison"),
	i04_Food_Chicken UMETA(Displayname = "i04_Food_Chicken"),
	i05_Equip_Axe_01 UMETA(Displayname = "i05_Equip_Axe_01"),
};