#ifndef LAIRETRIEVALHANDLER_HPP
#define LAIRETRIEVALHANDLER_HPP

#include "processorhandler.hpp"

class LaiRetrievalHandler : public ProcessorHandler
{
private:
    void HandleJobSubmittedImpl(EventProcessingContext &ctx,
                                const JobSubmittedEvent &event) override;
    void HandleTaskFinishedImpl(EventProcessingContext &ctx,
                                const TaskFinishedEvent &event) override;

    void CreateNewProductInJobTasks(QList<TaskToSubmit> &outAllTasksList, int nbProducts);
    void HandleNewProductInJob(EventProcessingContext &ctx, int jobId, const QString &jsonParams,
                               const QStringList &listProducts);
    void GetModelFileList(QStringList &outListModels, const QString &strPattern, std::map<QString, QString> &configParameters);
    void WriteExecutionInfosFile(const QString &executionInfosPath, const QJsonObject &parameters,
                                std::map<QString, QString> &configParameters,
                                const QStringList &listProducts);

    // Arguments getters
    QStringList GetNdviRviExtractionArgs(const QString &inputProduct, const QString &ftsFile, const QString &resolution);
    QStringList GetLaiModelExtractorArgs(const QString &inputProduct, const QStringList &modelsList, const QStringList &errModelsList,
                                         const QString &modelFileName, const QString &errModelFileName);
    QStringList GetBvImageInvArgs(const QString &ftsFile, const QString &modelFileName, const QString &monoDateLaiFileName);
    QStringList GetBvErrImageInvArgs(const QString &ftsFile, const QString &errModelFileName, const QString &monoDateErrFileName);
    QStringList GetMonoDateMskFagsArgs(const QString &inputProduct, const QString &monoDateMskFlgsFileName);
    QStringList GetTimeSeriesBuilderArgs(const QStringList &monoDateLaiFileNames, const QString &allLaiTimeSeriesFileName);
    QStringList GetErrTimeSeriesBuilderArgs(const QStringList &monoDateErrLaiFileNames, const QString &allErrTimeSeriesFileName);
    QStringList GetMskFlagsTimeSeriesBuilderArgs(const QStringList &monoDateMskFlagsLaiFileNames, const QString &allMskFlagsTimeSeriesFileName);
    QStringList GetProfileReprocessingArgs(std::map<QString, QString> configParameters, const QString &allLaiTimeSeriesFileName,
                                           const QString &allErrTimeSeriesFileName, const QString &reprocTimeSeriesFileName,
                                           const QStringList &listProducts);
    QStringList GetReprocProfileSplitterArgs(const QString &reprocTimeSeriesFileName, const QString &reprocFileListFileName);
    QStringList GetFittedProfileReprocArgs(const QString &allLaiTimeSeriesFileName, const QString &allErrTimeSeriesFileName,
                                           const QString &fittedTimeSeriesFileName, const QStringList &listProducts);
    QStringList GetFittedProfileReprocSplitterArgs(const QString &fittedTimeSeriesFileName, const QString &fittedFileListFileName);
    QStringList GetProductFormatterArgs(TaskToSubmit &productFormatterTask, std::map<QString, QString> configParameters,
                                        const QJsonObject &parameters, const QStringList &listProducts,
                                        const QString &reprocTimeSeriesFileName, const QString &fittedTimeSeriesFileName);
};

#endif // LAIRETRIEVALHANDLER_HPP
