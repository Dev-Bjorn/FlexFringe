include_directories("${CMAKE_CURRENT_SOURCE_DIR}/evaluation"
        "${CMAKE_CURRENT_SOURCE_DIR}/utility"
        )

## create the gitversion.cpp
## check version and append either version or "No commit info available\";"
execute_process(COMMAND "git" "rev-parse" "HEAD"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE Gitversion)
if (Gitversion STREQUAL "")
    set(Gitversion "No commit info available")
    message(${Gitversion})
endif ()
string(REPLACE "\n" "" Gitversion ${Gitversion})
file(WRITE "gitversion.cpp" "const char *gitversion = \"${Gitversion}\";")

## create the evaluators.h file
file(GLOB Files "${CMAKE_CURRENT_SOURCE_DIR}/evaluation/*.h")
file(WRITE "evaluators.h" "#ifndef __ALL_HEADERS__ \n#define __ALL_HEADERS__ \n\n")
foreach (Filepath ${Files})
    get_filename_component(Filename ${Filepath} NAME)
    file(APPEND "evaluators.h" "#include \"${Filename}\"\n")
endforeach ()
file(APPEND "evaluators.h" "\n#endif")

add_library(Source STATIC
        apta.cpp
        apta.h
        common.h
        conflict_graph.cpp
        conflict_graph.h
        dfasat.cpp
        dfasat.h
        evaluation_factory.cpp
        evaluation_factory.h
        evaluators.h
        gitversion.cpp
        interactive.cpp
        interactive.h
        parameters.cpp
        parameters.h
        greedy.cpp
        greedy.h
        refinement.cpp
        refinement.h
        searcher.cpp
        searcher.h
        state_merger.cpp
        state_merger.h
        stream.cpp
        stream.h
        mem_store.cpp
        mem_store.h
        predict.cpp predict.h
        differencing.cpp differencing.h
        ensemble.cpp ensemble.h
        dfa_properties.cpp dfa_properties.h
        input/trace.cpp
        input/trace.h
        input/inputdatalocator.cpp
        input/inputdatalocator.h
        input/tail.cpp input/tail.h
        input/attribute.cpp
        input/attribute.h
        input/abbadingoreader.cpp
        input/abbadingoreader.h
        input/parsers/csvparser.h
        input/parsers/csvparser.cpp
        input/parsers/i_parser.h
        input/inputdata.h
        input/inputdata.cpp
        input/parsers/symbol_info.cpp
        input/parsers/symbol_info.h
        input/parsers/abbadingoparser.cpp
        input/parsers/abbadingoparser.h
        input/parsers/grammar/abbadingoheader.h
        input/parsers/grammar/abbadingosymbol.h
        input/parsers/attribute_info.cpp
        input/parsers/attribute_info.h
        input/parsers/grammar/csvheader.h
        input/parsers/dummyparser.cpp
        input/parsers/dummyparser.h
        input/streaming.cpp
        input/streaming.h
        input/parsers/reader_strategy.cpp
        input/parsers/reader_strategy.h
        mcts/Random.h
        mcts/Random.cpp
        mcts/node/MCTSNode.h
        mcts/node/MCTSNode.cpp
        mcts/MCTS.h
        mcts/MCTS.cpp
        mcts/selection/node/LCB1SelectionPolicy.h
        mcts/selection/node/LCB1SelectionPolicy.cpp
        mcts/selection/node/NodeSelection.cpp
        mcts/selection/node/NodeSelection.h
        mcts/propagate/data/LUCB1Data.h
        mcts/propagate/data/LUCB1Data.cpp
        mcts/action/UniformMergeSplitFirst.h
        mcts/action/UniformMergeSplitFirst.cpp
        mcts/action/ActionSelectionPolicy.h
        mcts/action/ActionSelectionPolicy.cpp
        mcts/action/FirstAction.cpp
        mcts/action/FirstAction.h
        mcts/propagate/data/NodeData.h
        mcts/node/MCTSNodeFactory.cpp
        mcts/node/MCTSNodeFactory.h
        mcts/propagate/data/NodeDataFactory.cpp
        mcts/propagate/data/NodeDataFactory.h
        mcts/propagate/ModelSizeEvaluator.cpp
        mcts/propagate/ModelSizeEvaluator.h
        mcts/propagate/QualityEvaluation.cpp
        mcts/propagate/QualityEvaluation.h
        mcts/action/WeightedMergeSplitFirst.cpp
        mcts/action/WeightedMergeSplitFirst.h
        mcts/Runner.cpp
        mcts/Runner.h
        mcts/node/printer/DotPrinter.cpp
        mcts/node/printer/DotPrinter.h
        mcts/node/printer/JSONPrinter.cpp
        mcts/node/printer/JSONPrinter.h
        mcts/node/printer/MCTSPrinter.cpp
        mcts/node/printer/MCTSPrinter.h
        config/Config.h
        config/MCTSConfig.h
        mcts/expansion/ExpansionRulePolicy.cpp
        mcts/expansion/ExpansionRulePolicy.h
        mcts/expansion/FullRule.cpp
        mcts/expansion/FullRule.h
        mcts/Strings.h
        Printer.cpp
        printer.h
        config/Config.cpp
        config/MCTSConfig.cpp
        mcts/comparison/Algorithm.cpp
        mcts/comparison/Algorithm.h
        mcts/comparison/Greedy.cpp
        mcts/comparison/Greedy.h
        mcts/expansion/SingleExtendRule.cpp
        mcts/expansion/SingleExtendRule.h
        mcts/action/GreedyAction.cpp
        mcts/action/GreedyAction.h
        mcts/selection/SelectionSearchPolicy.cpp
        mcts/selection/SelectionSearchPolicy.h
        mcts/selection/BFSSearch.cpp
        mcts/selection/BFSSearch.h
        mcts/selection/ChildVSParentSearch.cpp
        mcts/selection/ChildVSParentSearch.h
        mcts/selection/node/UCB1SelectionPolicy.cpp
        mcts/selection/node/UCB1SelectionPolicy.h
        mcts/Refinements.h
        mcts/action/UniformAction.cpp
        mcts/action/UniformAction.h
        mcts/convergence/ConvergencePolicy.cpp
        mcts/convergence/ConvergencePolicy.h
        mcts/convergence/IterationConvergence.cpp
        mcts/convergence/IterationConvergence.h
        mcts/convergence/ScoreThresholdConvergence.cpp
        mcts/convergence/ScoreThresholdConvergence.h
        mcts/convergence/ScoreImprovementConvergence.cpp
        mcts/convergence/ScoreImprovementConvergence.h
        mcts/AlgorithmTypes.h
        mcts/AlgorithmTypes.cpp
        mcts/action/WeightedAction.cpp
        mcts/action/WeightedAction.h
        mcts/convergence/NodeTerminalConvergence.cpp
        mcts/convergence/NodeTerminalConvergence.h
        mcts/Refinements.cpp
        mcts/node/RolloutData.cpp
        mcts/node/RolloutData.h
        mcts/propagate/RolloutLength.cpp
        mcts/propagate/RolloutLength.h
        mcts/propagate/PathLength.cpp
        mcts/propagate/PathLength.h
        mcts/propagate/ExtendAmount.cpp
        mcts/propagate/ExtendAmount.h
        mcts/propagate/InterpolateSizeAndLength.h
        mcts/propagate/InterpolateSizeAndLength.cpp
        mcts/StringReader.cpp
        mcts/StringReader.h
)

## create CMakeLists.txt in evaluation directory
#set(cmakelists_eval "evaluation/CMakeLists.txt")
#file(WRITE "${cmakelists_eval}" "add_library(Evaluation STATIC")
#file(GLOB CPPFiles "${CMAKE_CURRENT_SOURCE_DIR}/evaluation/*.cpp")
#foreach (Filepath ${Files})
#    get_filename_component(Filename ${Filepath} NAME)
#    string(REGEX REPLACE "\\.[^.]*$" "" CleanFilename ${Filename})
#    file(APPEND "${cmakelists_eval}" "\n            ${CleanFilename}.h")
#    file(APPEND "${cmakelists_eval}" "\n            ${CleanFilename}.cpp")
#endforeach ()
#file(APPEND "${cmakelists_eval}" ")")

add_subdirectory(evaluation)
add_subdirectory(utility)
