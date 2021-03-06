#pragma once

#include "abstractGenerator.h"

namespace ubiq {
namespace generator {

/** Generator for server-side part of an Ubiq application, Dispatcher class.
  It is able to receive messages from various sources, process them and send the result
  back. It doesn't have state, so is not able to support multiphase protocols.
  */
class DispatcherGenerator : public AbstractGenerator
{
public:
	/** Constructor.
	  @param templateDirPath Path to a directory with generation template.
	  @param outputDirPath Path to a directory where DeviceDispatcher.cs shall be generated.
	  @param logicalModel Logical model reference.
	  @param errorReporter Object to return errors to.
	  */
	DispatcherGenerator(QString const &templateDirPath
			, QString const &outputDirPath
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

	virtual ~DispatcherGenerator();

	/// Starts generation process.
	virtual void generate();

private:
	/** Helper structure to contain the result of generation of code branch.
	  When generator reaches DecisionNode (corresponding to If statement), it
	  runs generation for Then and for Else branches, each of them is stopping
	  on Merge node or end of program. This structure contains text generated by
	  a branch and a node where generation has stopped.
	  */
	struct CodeBranchGenerationResult
	{
		CodeBranchGenerationResult(QString text, qReal::Id stopNode)
				:text(text), stopNode(stopNode)
		{
		}

		/// Branch generation result.
		QString text;

		/// Node where generation of a branch has stopped.
		qReal::Id stopNode;
	};

	/// Generates handlers for all message sources that dispatcher is able to process.
	/// Handlers are declared on a diagram using Handler element.
	QString generateEventHandlers(qReal::Id const &diagram) const;

	/// Generates code for message preprocessors (Ubiq framework methods that transforms
	/// a message somehow before passing it to a handler, for each handler there can be
	/// separate preprocessor).
	QString generatePreprocessors(qReal::Id const &diagram) const;

	/// Generates handler code (switch statement by message type)
	QString generateEventHandler(QString const &handlerName) const;

	/// Generates message preprocessor.
	QString generatePreprocessor(QString const &handlerName) const;

	/// Generates code for HandlerStart node and starts generation of body.
	QString generateCaseBody(qReal::Id const &handlerStart) const;

	/// Generates code for chain of the statements or branch of decision node.
	/// Stops generation on Merge node or at the end of the branch.
	CodeBranchGenerationResult generateOperatorCode(qReal::Id const &currentNode) const;

	/// Generates constants declared in a dispatcher class.
	QString generateConstants(qReal::Id const &element) const;

	/// Generates custom fields of a dispatcher class.
	QString generateFields(qReal::Id const &element) const;

	/// Generates code that describes message input method in a Dispatcher constructor.
	QString generateMessageInputMethods(qReal::Id const &element) const;

	/// Generates handcoded "Helper" functions.
	QString generateHelperFunctions(qReal::Id const &element) const;

	/// Generates formal parameters of a helper function.
	QString generateFunctionParameters(qReal::Id const &element) const;

	/// Generates body of a helper function (the same as case body).
	QString generateFunctionBody(qReal::Id const &element) const;

	/// Generates code for functional calls elements.
	CodeBranchGenerationResult generateFunctionCallCode(qReal::Id const &currentNode) const;

	/// Generates code for action nodes.
	CodeBranchGenerationResult generateActionCode(qReal::Id const &currentNode) const;

	/// Generates code for initial nodes.
	CodeBranchGenerationResult generateInitialNodeCode(qReal::Id const &currentNode) const;

	/// Generates code for return action nodes.
	CodeBranchGenerationResult generateReturnActionCode(qReal::Id const &currentNode) const;

	/// Generates code for decision nodes.
	CodeBranchGenerationResult generateDecisionNodeCode(qReal::Id const &currentNode) const;
};

}
}
