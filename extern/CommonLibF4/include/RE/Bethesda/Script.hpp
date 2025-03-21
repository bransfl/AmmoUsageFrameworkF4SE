#pragma once

#include "RE/Bethesda/BSString.hpp"
#include "RE/Bethesda/BSTList.hpp"
#include "RE/Bethesda/MemoryManager.hpp"
#include "RE/Bethesda/TESForms.hpp"

namespace RE
{
	enum class COMPILER_NAME
	{
		kDefault,
		kSystemWindow,
		kDialogue
	};

	enum class SCRIPT_PARAM_TYPE
	{
		kChar,
		kInt,
		kFloat,
		kInventoryObject,
		kObjectRef,
		kActorValue,
		kActor,
		kSpellItem,
		kAxis,
		kCell,
		kAnimGroup,
		kMagicItem,
		kSound,
		kTopic,
		kQuest,
		kRace,
		kClass,
		kFaction,
		kSex,
		kGlobal,
		kFurnitureOrFormList,
		kObject,
		kScriptVar,
		kStage,
		kMapMarker,
		kActorBase,
		kContainerRef,
		kWorlOrList,
		kCrimeType,
		kPackage,
		kCombatStyle,
		kMagicEffect,
		kFormType,
		kWeather,
		kNPC,
		kOwner,
		kShaderEffect,
		kFormList,
		kMenuIcon,
		kPerk,
		kNote,
		kMiscStat,
		kImageSpaceMod,
		kImageSpace,
		kVATSValue,
		kVATSValueData,
		kEventFunction,
		kEventFunctionMember,
		kEventFunctionData,
		kVoiceType,
		kEncounterZone,
		kIdleForm,
		kMessage,
		kInvObjectOrFormList,
		kAlignment,
		kEquipType,
		kObjectOrFormList,
		kMusic,
		kCritStage,
		kKeyword,
		kRefType,
		kLocation,
		kForm,
		kAlias,
		kShout,
		kWordOfPower,
		kRelationshipRank,
		kBGSScene,
		kCastingSource,
		kAssociationType,
		kWardState,
		kPackageDataCanBeNull,
		kPackageDataNumeric,
		kPackageDataReference,
		kVMScriptVar,
		kReferenceEffect,
		kPackageDataLocation,
		kSoundCategory,
		kKnowableForm,
		kRegion,
		kAction,
		kMovementIdleFromState,
		kMovementIdleToState,
		kVMRefOrAliasScript,
		kDamageType,
		kSceneAction,
		kKeywordOrFormlist,
		kFurnEntryType
	};

	enum class SCRIPT_ERROR;
	enum class SCRIPT_OUTPUT;

	class BGSPackageDataList;
	class BGSStoryEvent;

	struct SCRIPT_PARAMETER
	{
	public:
		// members
		const char* paramName{ "" };						   // 00
		REX::Enum<SCRIPT_PARAM_TYPE, std::uint32_t> paramType; // 08
		bool optional{ false };								   // 0C
	};
	static_assert(sizeof(SCRIPT_PARAMETER) == 0x10);

	struct ACTION_OBJECT
	{
	public:
		// members
		TESForm* form;		 // 00
		std::uint32_t flags; // 08
	};
	static_assert(sizeof(ACTION_OBJECT) == 0x10);

	struct SCRIPT_LOCAL
	{
	public:
		// members
		std::uint32_t id; // 00
		float value;	  // 04
		bool isInteger;	  // 08
	};
	static_assert(sizeof(SCRIPT_LOCAL) == 0xC);

	struct SCRIPT_EFFECT_DATA
	{
	public:
		// members
		bool scriptEffectStart;	 // 00
		bool scriptEffectFinish; // 01
		float secondsElapsed;	 // 04
	};
	static_assert(sizeof(SCRIPT_EFFECT_DATA) == 0x8);

	class ScriptLocals
	{
	public:
		// members
		Script* masterScript;					  // 00
		std::int8_t flags;						  // 08
		BSSimpleList<ACTION_OBJECT*>* actionList; // 10
		BSSimpleList<SCRIPT_LOCAL*>* localList;	  // 18
		SCRIPT_EFFECT_DATA* scriptEffectData;	  // 20
	};
	static_assert(sizeof(ScriptLocals) == 0x28);

	struct SCRIPT_LINE
	{
	public:
		// members
		std::uint32_t lineNumber;							// 000
		char line[512];										// 004
		std::uint32_t size;									// 204
		std::uint32_t offset;								// 208
		char output[512];									// 20C
		std::uint32_t outputSize;							// 40C
		SCRIPT_OUTPUT expression;							// 410
		std::uint32_t refObjectIndex;						// 414
		REX::Enum<SCRIPT_ERROR, std::uint32_t> scriptError; // 418
	};
	static_assert(sizeof(SCRIPT_LINE) == 0x41C);

	struct SCRIPT_HEADER
	{
	public:
		// members
		std::uint32_t variableCount;  // 00
		std::uint32_t refObjectCount; // 04
		std::uint32_t dataSize;		  // 08
		std::uint32_t lastID;		  // 0C
		bool isQuestScript;			  // 10
		bool isMagicEffectScript;	  // 11
		bool isCompiled;			  // 12
	};
	static_assert(sizeof(SCRIPT_HEADER) == 0x14);

	struct ScriptVariable
	{
	public:
		// members
		SCRIPT_LOCAL data; // 00
		BSString name;	   // 10
	};
	static_assert(sizeof(ScriptVariable) == 0x20);

	struct SCRIPT_REFERENCED_OBJECT
	{
	public:
		// members
		BSString editorID;		  // 00
		TESForm* form;			  // 10
		std::uint32_t variableID; // 18
	};
	static_assert(sizeof(SCRIPT_REFERENCED_OBJECT) == 0x20);

	class ScriptCompileData
	{
	public:
		// members
		const char* input;										// 00
		std::uint32_t inputOffset;								// 08
		REX::Enum<COMPILER_NAME, std::uint32_t> compilerIndex;	// 0C
		BSString scriptName;									// 10
		SCRIPT_ERROR lastError;									// 20
		bool isPartialScript;									// 24
		std::uint32_t uiLastLineNumber;							// 28
		char* output;											// 30
		std::uint32_t outputOffset;								// 38
		SCRIPT_HEADER header;									// 3C
		BSSimpleList<ScriptVariable*> listVariables;			// 50
		BSSimpleList<SCRIPT_REFERENCED_OBJECT*> listRefObjects; // 60
		Script* currentScript;									// 70
		BSSimpleList<SCRIPT_LINE*> listLines;					// 78
	};
	static_assert(sizeof(ScriptCompileData) == 0x88);

	struct ConditionCheckParams
	{
	public:
		// members
		TESObjectREFR* actionRef{ nullptr };					 // 00
		TESObjectREFR* targetRef{ nullptr };					 // 08
		const TESQuest* scopeQuest{ nullptr };					 // 10
		const BGSStoryEvent* scopeEvent{ nullptr };				 // 18
		Actor* scopeActor{ nullptr };							 // 20
		const BGSPackageDataList* runningPackageData{ nullptr }; // 28
		BGSMod::Template::Item* objectTemplateItem{ nullptr };	 // 30
		const TESForm* extraForms[5]{ nullptr };				 // 38
		std::uint32_t extraFormCount{ 0 };						 // 60
		bool outDispFailure{ false };							 // 64
	};
	static_assert(sizeof(ConditionCheckParams) == 0x68);

	struct SCRIPT_FUNCTION
	{
	public:
		using ConditionFunction_t = bool(ConditionCheckParams& a_data, void* a_param2, void* a_param1, float& a_returnValue);
		using CompileFunction_t = bool(const std::uint16_t a_paramCount, const SCRIPT_PARAMETER* a_parameters, SCRIPT_LINE* a_scriptLine, ScriptCompileData* a_compileData);
		using ExecuteFunction_t = bool(const SCRIPT_PARAMETER* a_parameters, const char* a_compiledParams, TESObjectREFR* a_refObject, TESObjectREFR* a_container, Script* a_script, ScriptLocals* a_scriptLocals, float& a_returnValue, std::uint32_t& a_offset);

		[[nodiscard]] static std::span<SCRIPT_FUNCTION> GetConsoleFunctions()
		{
			static REL::Relocation<SCRIPT_FUNCTION(*)[]> functions{ REL::RelocationID(901511, 901511) };
			static std::size_t SIZE = REL::Module::IsNG() ? 526 : 522;
			return { *functions, SIZE };
		}

		[[nodiscard]] static std::span<SCRIPT_FUNCTION, 819> GetScriptFunctions()
		{
			static REL::Relocation<SCRIPT_FUNCTION(*)[819]> functions{ REL::RelocationID(75173, 75173) };
			return { *functions };
		}

		// members
		const char* functionName{ "" };																						   // 00
		const char* shortName{ "" };																						   // 08
		REX::Enum<SCRIPT_OUTPUT, std::uint32_t> output;																		   // 10
		const char* helpString{ "" };																						   // 18
		bool referenceFunction{ false };																					   // 20
		std::uint16_t paramCount{ 0 };																						   // 22
		SCRIPT_PARAMETER* parameters{ nullptr };																			   // 28
		ExecuteFunction_t* executeFunction{ nullptr };																		   // 30
		CompileFunction_t* compileFunction{ REL::Relocation<CompileFunction_t*>{ REL::RelocationID(638606, 2204343) }.get() }; // 38
		ConditionFunction_t* conditionFunction{ nullptr };																	   // 40
		bool editorFilter{ false };																							   // 48
		bool invalidatesCellList{ false };																					   // 49
	};
	static_assert(sizeof(SCRIPT_FUNCTION) == 0x50);

	class ScriptCompiler
	{
	public:
	};
	static_assert(std::is_empty_v<ScriptCompiler>);

	class __declspec(novtable) Script
		: public TESForm // 00
	{
	public:
		inline static constexpr auto RTTI{ RTTI::Script };
		inline static constexpr auto VTABLE{ VTABLE::TESForm };
		inline static constexpr auto FORMTYPE{ ENUM_FORMTYPE::kSCPT };

		template <class... Args>
		static bool ParseParameters(const SCRIPT_PARAMETER* a_parameters, const char* a_compiledParams, std::uint32_t& a_offset, TESObjectREFR* a_refObject, TESObjectREFR* a_container, Script* a_script, ScriptLocals* a_scriptLocals, Args... a_args)
		{
			static_assert((std::is_pointer_v<Args> && ...), "arguments must all be pointers");
			using func_t = bool(const SCRIPT_PARAMETER*, const char*, std::uint32_t&, TESObjectREFR*, TESObjectREFR*, Script*, ScriptLocals*, ...);
			static REL::Relocation<func_t> func{ REL::RelocationID(1607, 2204298) };
			return func(a_parameters, a_compiledParams, a_offset, a_refObject, a_container, a_script, a_scriptLocals, a_args...);
		}

		void CompileAndRun(ScriptCompiler* a_compiler, COMPILER_NAME a_compilerIndex, TESObjectREFR* a_ownerObject)
		{
			using func_t = decltype(&Script::CompileAndRun);
			static REL::Relocation<func_t> func{ REL::RelocationID(526625, 2204287) };
			return func(this, a_compiler, a_compilerIndex, a_ownerObject);
		}

		static bool GetProcessScripts()
		{
			using func_t = decltype(&Script::GetProcessScripts);
			static REL::Relocation<func_t> func{ REL::RelocationID(44950, 2204310) };
			return func();
		}

		static void SetProcessScripts(bool a_processScripts)
		{
			using func_t = decltype(&Script::SetProcessScripts);
			static REL::Relocation<func_t> func{ REL::RelocationID(1188642, 2204309) };
			return func(a_processScripts);
		}

		void SetText(std::string_view a_text)
		{
			if (text) {
				free(text);
				text = nullptr;
			}

			text = calloc<char>(a_text.length() + 1);
			std::memset(text, '\0', a_text.length() + 1);
			std::memcpy(text, a_text.data(), a_text.length());
		}

		// members
		SCRIPT_HEADER header;								// 20
		char* text;											// 38
		std::byte* data;									// 40
		float profilerTimer;								// 48
		float questScriptDelay;								// 4C
		float questScriptGetSecondsBuffer;					// 50
		TESQuest* parentQuest;								// 58
		BSSimpleList<SCRIPT_REFERENCED_OBJECT*> refObjects; // 60
		BSSimpleList<ScriptVariable*> variables;			// 70
	};
	static_assert(sizeof(Script) == 0x80);
}
