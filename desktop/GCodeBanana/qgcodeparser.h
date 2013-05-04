#ifndef QGCODEPARSER_H
#define QGCODEPARSER_H

#include <QObject>
#include <QFile>
#include "qearleyparser.h"
#include "codeeditor.h"

typedef struct {
    QStringList functions;                          /// a list of functions to call
} GCodeRule;

class QGCodeParser : public QObject
{
    Q_OBJECT
public:
    explicit QGCodeParser(CodeEditor *editor, QObject *parent = 0);

private:
    QEarleyParser               *earleyParser;                                  /// the earley parser
    QList<int>                  expressionWhitespaceList;                       /// this list holds count of removed whitespace for each character of the expression
    QHash<QString, GCodeRule>   gcodeRules;                                     /// map of all rules, key is rule
    QStringList                 oldLines;

    CodeEditor                  *codeEditor;

    /** initializes the parser */
    void initialize();
    /** loads the grammar from a file */
    void loadGrammar(QString fileName);
    /** loads expression from a file */
    void loadExpression(QString fileName);
    /** evaluates an expression */
    bool evaluateExpression(QString text);
    /** adds a rule */
    void addRule(QString rule, QString functions);
    /** strips all comments from the text */
    QString stripComments(QString text);
    /** removes the whitespace of a string and saves the count */
    QString removeWhitespace(QString text, QList<int> *whiteSpaceList);

signals:
    void codeChanged(bool valid);
    
private slots:

    /** evaluates the code of the textEditor */
    void evaluateCode();
    
};

#endif // QGCODEPARSER_H
