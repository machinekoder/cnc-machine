#include "qgcodeparser.h"

QGCodeParser::QGCodeParser(CodeEditor *editor, QObject *parent) :
    QObject(parent)
{
    initialize();

    codeEditor = editor;
    connect(editor, SIGNAL(textChanged()),
            this, SLOT(evaluateCode()));
}

QString QGCodeParser::strippedCode()
{
    QStringList newLines = codeEditor->toPlainText().split("\n");
    for (int i = 0; i < newLines.size(); ++i)
    {
        newLines.replace(i, stripComments(newLines.at(i)));
    }

    return newLines.join('\n');
}

bool QGCodeParser::hasError()
{
    return m_hasError;
}

void QGCodeParser::initialize()
{
    // initialize variables
    m_hasError = false;

    // create earley parser
    earleyParser = new QEarleyParser(this);

    loadGrammar(":/settings/grammar");
    earleyParser->setStartSymbol("S");
}

void QGCodeParser::loadGrammar(QString fileName)
{
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QStringList lines = QString::fromUtf8(file.readAll()).split('\n');
        foreach (QString line, lines)
        {
            if (line.trimmed().isEmpty() || (line.trimmed().at(0) == '#'))
                continue;

            if (line.contains("//"))
                line.truncate(line.indexOf("//"));

            QStringList ruleData = line.split(';');
            QString rule;
            QString functions;

            for (int i = ruleData.size()-2; i >= 0; i--)    // handle termination of ;
            {
                if (ruleData.at(i).at(ruleData.at(i).size()-1) == '\\')
                {
                    if (ruleData.size() > (i+1))
                    {
                        ruleData[i].chop(1);
                        ruleData[i].append(';');
                        ruleData[i].append(ruleData.at(i+1));
                        ruleData.removeAt(i+1);
                    }
                }
            }

            rule = ruleData.at(0).trimmed();
            if (ruleData.size() > 1)
                functions = ruleData.at(1).trimmed();

            addRule(rule, functions);
        }
    }
    else
        qFatal("Can't open file");
}

void QGCodeParser::loadExpression(QString fileName)
{
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString text(file.readAll());
        QStringList lines = text.split("\n");

        foreach (QString line, lines)
        {
            qDebug() << line;
            qDebug() << evaluateExpression(line);
        }
    }
    else
        qWarning("Can't open file");
}

bool QGCodeParser::evaluateExpression(QString text)
{
    text = stripComments(text).trimmed();
    //text = removeWhitespace(text, &expressionWhitespaceList);
    return earleyParser->parseWord(text);
}

void QGCodeParser::addRule(QString rule, QString functions)
{
    GCodeRule gcodeRule;
    if (!functions.isEmpty())
        gcodeRule.functions = functions.split(',');
    gcodeRules.insert(rule, gcodeRule);

    QStringList ruleFunctions;
    foreach (QString function, gcodeRule.functions)
        ruleFunctions.append(function.trimmed());
    earleyParser->loadRule(rule, ruleFunctions);
}

QString QGCodeParser::stripComments(QString text)
{
    //strip singe line comment
    int pos;// = text.indexOf("//");
    //if (pos != -1)
    //    text = text.left(pos);
    //strip multi line comments
    pos = text.indexOf("(");
    if (pos != -1)
        text = text.left(pos);
    pos = text.indexOf(")");
    if (pos != -1)
        text = text.mid(pos+1);
    return text;
}

QString QGCodeParser::removeWhitespace(QString text, QList<int> *whiteSpaceList)
{
    int whitespaceCount = 0;
    QString output;

    whiteSpaceList->clear();
    for (int i = text.size() - 1; i >= 0; i--)
    {
        if (text.at(i).isSpace())
            whitespaceCount++;
        else
        {
            output.prepend(text.at(i));
            whiteSpaceList->prepend(whitespaceCount);
        }
    }

    for (int i = 0; i < whiteSpaceList->size(); i++)
        (*whiteSpaceList)[i] = whitespaceCount - whiteSpaceList->at(i);

    return output;
}

void QGCodeParser::evaluateCode()
{
    QStringList newLines = codeEditor->toPlainText().split("\n");
    bool globalResult = true;

    for (int i = 0; i < newLines.count(); ++i)
    {
        if ((oldLines.count() != newLines.count()) || ((oldLines.count() > i) && (newLines.at(i) != oldLines.at(i))))
        {
            bool result = evaluateExpression(newLines.at(i));
            codeEditor->setLineError(i, !result);

            if (result != true)
                globalResult = false;
        }
    }

    oldLines = newLines;
    m_hasError = !globalResult;

    emit codeChanged(globalResult);
}
