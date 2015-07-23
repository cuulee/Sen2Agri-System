#ifndef PERSISTENCEITFMODULE_H
#define PERSISTENCEITFMODULE_H

#include "persistencemanager_interface.h"
#include "processorexecutioninfos.h"

/**
 * @brief The PersistenceItfModule class
 * \note
 * This class represents the interface with the persistence manager
 * application. The communication is made via DBus.
 *
 */

class PersistenceItfModule : public QObject
{
    Q_OBJECT

public:
    ~PersistenceItfModule();

    static PersistenceItfModule *GetInstance();

    void SendProcessorExecInfos(ProcessorExecutionInfos &execInfos);
    void RequestConfiguration();

    void MarkStepPendingStart(int taskId, QString &name);
    void MarkStepStarted(int taskId, QString &name);
    bool MarkStepFinished(int taskId, QString &name, ProcessorExecutionInfos &statistics);

signals:
    void OnConfigurationReceived();

private:
    PersistenceItfModule();
    OrgEsaSen2agriPersistenceManagerInterface clientInterface;

    bool GetProcessorPathForName(const ConfigurationParameterValueList &configuration,
                                const QString &name, QString &path);
    void SaveMainConfigKeys(const ConfigurationParameterValueList &configuration);
    void SaveProcessorsConfigKeys(const ConfigurationParameterValueList &configuration);
    long ParseTimeStr(QString &strTime);
};

#endif // PERSISTENCEITFMODULE_H
