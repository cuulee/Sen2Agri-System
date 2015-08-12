#ifndef UPDATESYNTHESIS_H
#define UPDATESYNTHESIS_H

typedef enum {SENSOR_S2, SENSOR_L8} SensorType;
typedef enum {RES_10M, RES_20M} ResolutionType;

typedef enum {LAND=1, WATER,SNOW,CLOUD,CLOUD_SHADOW} FlagType;

#define FLAG_NO_DATA -1.0f
#define DATE_NO_DATA -1.0f
#define REFLECTANCE_NO_DATA -1.0f
#define WEIGHT_NO_DATA -1.0f

#define S2_L2A_10M_BANDS_NO     4
#define L8_L2A_10M_BANDS_NO     3

#define WEIGHTED_REFLECTANCE_10M_BANDS_NO   S2_L2A_10M_BANDS_NO

#define S2_L2A_10M_BANDS_START_IDX      0
#define S2_L2A_10M_BLUE_BAND_IDX        0
#define S2_L2A_10M_RED_BAND_IDX         2
#define S2_L2A_10M_CLD_MASK_IDX         4
#define S2_L2A_10M_WATER_MASK_IDX       5
#define S2_L2A_10M_SNOW_MASK_IDX        6
#define S2_L2A_10M_TOTAL_WEIGHT_IDX     7
#define S2_L2A_10M_L3A_WEIGHT_START_IDX 8
#define S2_L2A_10M_L3A_W_AV_DATE_IDX    12
#define S2_L2A_10M_L3A_REFL_START_IDX   13
#define S2_L2A_10M_L3A_PIXEL_STATUS_IDX 17

#define L8_L2A_10M_BANDS_START_IDX      0
#define L8_L2A_10M_BLUE_BAND_IDX        0
#define L8_L2A_10M_RED_BAND_IDX         2
#define L8_L2A_10M_CLD_MASK_IDX         3
#define L8_L2A_10M_WATER_MASK_IDX       4
#define L8_L2A_10M_SNOW_MASK_IDX        5
#define L8_L2A_10M_TOTAL_WEIGHT_IDX     6
#define L8_L2A_10M_L3A_WEIGHT_START_IDX 7
#define L8_L2A_10M_L3A_W_AV_DATE_IDX    11
#define L8_L2A_10M_L3A_REFL_START_IDX   12
#define L8_L2A_10M_L3A_PIXEL_STATUS_IDX 16

// 20M Positions Definition
#define S2_L2A_20M_BANDS_NO     6
#define L8_L2A_20M_BANDS_NO     3
#define WEIGHTED_REFLECTANCE_20M_BANDS_NO   S2_L2A_20M_BANDS_NO

#define S2_L2A_20M_BANDS_START_IDX      0
#define S2_L2A_20M_BLUE_BAND_IDX        -1
#define S2_L2A_20M_RED_BAND_IDX         -1
#define S2_L2A_20M_CLD_MASK_IDX         6
#define S2_L2A_20M_WATER_MASK_IDX       7
#define S2_L2A_20M_SNOW_MASK_IDX        8
#define S2_L2A_20M_TOTAL_WEIGHT_IDX     9
#define S2_L2A_20M_L3A_WEIGHT_START_IDX 10
#define S2_L2A_20M_L3A_W_AV_DATE_IDX    16
#define S2_L2A_20M_L3A_REFL_START_IDX   17
#define S2_L2A_20M_L3A_PIXEL_STATUS_IDX 23

#define L8_L2A_20M_BANDS_START_IDX      0
#define L8_L2A_20M_BLUE_BAND_IDX        -1
#define L8_L2A_20M_RED_BAND_IDX         -1
#define L8_L2A_20M_CLD_MASK_IDX         3
#define L8_L2A_20M_WATER_MASK_IDX       4
#define L8_L2A_20M_SNOW_MASK_IDX        5
#define L8_L2A_20M_TOTAL_WEIGHT_IDX     6
#define L8_L2A_20M_L3A_WEIGHT_START_IDX 7
#define L8_L2A_20M_L3A_W_AV_DATE_IDX    13
#define L8_L2A_20M_L3A_REFL_START_IDX   14
#define L8_L2A_20M_L3A_PIXEL_STATUS_IDX 20

#define L3A_WEIGHTED_REFLECTANCES_MAX_NO       S2_L2A_20M_BANDS_NO

template< class TInput, class TOutput>
class UpdateSynthesisFunctor
{
private:
    SensorType m_sensorType;
    ResolutionType m_resolution;

    float m_fQuantificationValue;
    int m_nCurrentDate;

    bool m_bPrevL3ABandsAvailable;
    int m_nNbOfL3AReflectanceBands;
    int m_nNbL2ABands;

    int m_nL2ABandStartIndex;
    int m_nCloudMaskBandIndex;
    int m_nSnowMaskBandIndex;
    int m_nWaterMaskBandIndex;
    int m_nCurrentL2AWeightBandIndex;
    int m_nPrevL3AWeightBandStartIndex;
    int m_nPrevL3AWeightedAvDateBandIndex;
    int m_nPrevL3AReflectanceBandStartIndex;
    int m_nPrevL3APixelFlagBandIndex;
    int m_nRedBandIndex;
    int m_nBlueBandIndex;

    // output pixel values
    float m_CurrentWeightedReflectances[L3A_WEIGHTED_REFLECTANCES_MAX_NO];
    float m_CurrentPixelWeights[L3A_WEIGHTED_REFLECTANCES_MAX_NO];
    float m_fCurrentPixelFlag;
    float m_fCurrentPixelWeightedDate;

    void ResetCurrentPixelValues()
    {
        for(int i = 0; i<m_nNbOfL3AReflectanceBands; i++)
        {
            m_CurrentPixelWeights[i] = WEIGHT_NO_DATA;
            m_CurrentWeightedReflectances[i] = REFLECTANCE_NO_DATA;
        }
        m_fCurrentPixelFlag = FLAG_NO_DATA;
        m_fCurrentPixelWeightedDate = DATE_NO_DATA;
    }

    int GetAbsoluteL2ABandIndex(int index)
    {
        // we know that the L2A bands are always the first bands
        if(index < m_nNbL2ABands)
            return (m_nL2ABandStartIndex + index);
        return -1;
    }

    float GetL2AReflectanceForPixelVal(float fPixelVal)
    {
        return (fPixelVal/m_fQuantificationValue);
    }

    void HandleLandPixel(const TInput & A)
    {
        // we assume that the reflectance bands start from index 0
        for(int i = 0; i<m_nNbOfL3AReflectanceBands; i++)
        {
            m_fCurrentPixelFlag = LAND;

            // we will always have as output the number of reflectances equal or greater than
            // the number of bands in the current L2A raster for the current resolution
            int nCurrentBandIndex = GetAbsoluteL2ABandIndex(i);
            if(nCurrentBandIndex != -1)
            {
                // "if band is available in the case of LANDSAT 8, some bands are not available\"
                float fCurReflectance = GetL2AReflectanceForPixelVal(A[nCurrentBandIndex]);
                float fPrevReflect = GetPrevL3AReflectanceValue(A, i);
                float fPrevWeight = GetPrevL3AWeightValue(A, i);
                float fCurrentWeight = GetCurrentL2AWeightValue(A);
                float fPrevWeightedDate = GetPrevL3AWeightedAvDateValue(A);
                m_CurrentWeightedReflectances[i] = (fPrevWeight * fPrevReflect + fCurrentWeight * fCurReflectance) /
                        (fPrevWeight + fCurrentWeight);
                m_fCurrentPixelWeightedDate = (fPrevWeight * fPrevWeightedDate + fCurrentWeight * m_nCurrentDate) /
                        (fPrevWeight + fCurrentWeight);
                m_CurrentPixelWeights[i] = (fPrevWeight + fCurrentWeight);

            } else {
                // L2A band missing - as for LANDSAT 8
                m_CurrentWeightedReflectances[i] = GetPrevL3AReflectanceValue(A, i);
                m_CurrentPixelWeights[i] = GetPrevL3AWeightValue(A, i);
                if(IsRedBand(i))
                {
                    m_fCurrentPixelWeightedDate = GetPrevL3AWeightedAvDateValue(A);
                }
            }
        }
    }

    void HandleSnowOrWaterPixel(const TInput & A)
    {
        if(IsWaterPixel(A)) {
            m_fCurrentPixelFlag = WATER;
        } else {
            m_fCurrentPixelFlag = SNOW;
        }
        for(int i = 0; i<m_nNbOfL3AReflectanceBands; i++)
        {
            int nCurrentBandIndex = GetAbsoluteL2ABandIndex(i);
            // band available
            if(nCurrentBandIndex != -1)
            {
                float fPrevWeight = GetPrevL3AWeightValue(A, i);
                // if pixel never observed without cloud, water or snow
                if(fPrevWeight == 0) {
                    m_CurrentWeightedReflectances[i] = GetPrevL3AReflectanceValue(A, i);
                    m_CurrentPixelWeights[i] = 0;
                    if(IsRedBand(i))
                    {
                        m_fCurrentPixelWeightedDate = m_nCurrentDate;
                    }
                } else {
                    // pixel already observed cloud free, keep the previous weighted average
                    m_CurrentWeightedReflectances[i] = GetPrevL3AReflectanceValue(A, i);
                    m_CurrentPixelWeights[i] = fPrevWeight;
                    if(IsRedBand(i))
                    {
                        m_fCurrentPixelWeightedDate = GetPrevL3AWeightedAvDateValue(A);
                        m_fCurrentPixelFlag = LAND;
                    }

                }
            } else {
                // band not available, keep previous values
                m_CurrentWeightedReflectances[i] = GetPrevL3AReflectanceValue(A, i);
                m_CurrentPixelWeights[i] = GetPrevL3AWeightValue(A, i);
                // TODO: In algorithm says nothing about WDate
            }
        }
    }

    void HandleCloudOrShadowPixel(const TInput & A)
    {
        // if flagN-1 is no-data => replace nodata with cloud
        if(m_fCurrentPixelFlag == FLAG_NO_DATA)
        {
            for(int i = 0; i<m_nNbOfL3AReflectanceBands; i++)
            {
                int nCurrentBandIndex = GetAbsoluteL2ABandIndex(i);
                // band available
                if(nCurrentBandIndex != -1)
                {
                    m_CurrentWeightedReflectances[i] = GetL2AReflectanceForPixelVal(A[nCurrentBandIndex]);;
                    m_CurrentPixelWeights[i] = 0;
                    if(IsRedBand(i))
                    {
                        m_fCurrentPixelWeightedDate = m_nCurrentDate;
                        m_fCurrentPixelFlag = CLOUD;
                    }
                } else {
                    m_CurrentWeightedReflectances[i] = WEIGHT_NO_DATA;
                    m_CurrentPixelWeights[i] = 0;
                }
            }
        } else {
            if((m_fCurrentPixelFlag == CLOUD) || (m_fCurrentPixelFlag == CLOUD_SHADOW))
            {
                // get the blue band index
                int nBlueBandIdx = GetBlueBandIndex();
                // in 20m resolution we have no blue band, so we will do the following
                // only for 10m resolution
                if(nBlueBandIdx != -1)
                {
                    float fBlueReflectance = GetL2AReflectanceForPixelVal(A[nBlueBandIdx]);
                    for(int i = 0; i<m_nNbOfL3AReflectanceBands; i++)
                    {
                        float fCurReflectance = GetPrevL3AReflectanceValue(A, i);
                        if(fBlueReflectance < fCurReflectance)
                        {

                            int nCurrentBandIndex = GetAbsoluteL2ABandIndex(i);
                            // band available
                            if(nCurrentBandIndex != -1)
                            {
                                m_CurrentWeightedReflectances[i] = GetL2AReflectanceForPixelVal(A[nCurrentBandIndex]);
                                m_CurrentPixelWeights[i] = 0;
                                if(IsRedBand(i))
                                {
                                    m_fCurrentPixelWeightedDate = m_nCurrentDate;
                                    m_fCurrentPixelFlag = CLOUD;
                                }
                            } else {
                                m_CurrentWeightedReflectances[i] = GetPrevL3AReflectanceValue(A, i);
                                m_CurrentPixelWeights[i] = 0;
                            }
                        }
                    }
                }
            } else {
                m_fCurrentPixelFlag = GetPrevL3APixelFlagValue(A);
                for(int i = 0; i<m_nNbOfL3AReflectanceBands; i++)
                {
                    m_CurrentWeightedReflectances[i] = GetPrevL3AReflectanceValue(A, i);
                    m_fCurrentPixelWeightedDate = GetPrevL3AWeightedAvDateValue(A);
                    m_CurrentPixelWeights[i] = 0;
                }

            }
        }
    }

    bool IsSnowPixel(const TInput & A)
    {
        if(m_nSnowMaskBandIndex == -1)
            return false;

        int val = (int)static_cast<float>(A[m_nSnowMaskBandIndex]);
        return (val != 0);
    }

    bool IsWaterPixel(const TInput & A)
    {
        if(m_nWaterMaskBandIndex == -1)
            return false;

        int val = (int)static_cast<float>(A[m_nWaterMaskBandIndex]);
        return (val != 0);
    }

    bool IsCloudPixel(const TInput & A)
    {
        if(m_nCloudMaskBandIndex== -1)
            return false;

        int val = (int)static_cast<float>(A[m_nCloudMaskBandIndex]);
        return (val != 0);
    }

    bool IsLandPixel(const TInput & A)
    {
        return (!IsSnowPixel(A) && !IsWaterPixel(A) && !IsCloudPixel(A));
    }

    bool IsRedBand(int index)
    {
        if((m_nRedBandIndex != -1) && (index == m_nRedBandIndex)) {
            return true;
        }
        return false;
    }

    float GetCurrentL2AWeightValue(const TInput & A)
    {
        // TODO: Normally, this should not happen so we should log this error and maybe throw an exception
        if(m_nCurrentL2AWeightBandIndex == -1)
            return WEIGHT_NO_DATA;

        int val = (int)static_cast<float>(A[m_nCurrentL2AWeightBandIndex]);
        return (val != 0);
    }

    float GetPrevL3AWeightValue(const TInput & A, int offset)
    {
        if(!m_bPrevL3ABandsAvailable || m_nPrevL3AWeightBandStartIndex == -1)
            return WEIGHT_NO_DATA;

        int val = (int)static_cast<float>(A[m_nPrevL3AWeightBandStartIndex+offset]);
        return (val != 0);
    }

    float GetPrevL3AWeightedAvDateValue(const TInput & A)
    {
        if(!m_bPrevL3ABandsAvailable || m_nPrevL3AWeightedAvDateBandIndex == -1)
            return DATE_NO_DATA;

        int val = (int)static_cast<float>(A[m_nPrevL3AWeightedAvDateBandIndex]);
        return (val != 0);
    }

    float GetPrevL3AReflectanceValue(const TInput & A, int offset)
    {
        if(!m_bPrevL3ABandsAvailable || m_nPrevL3AReflectanceBandStartIndex == -1)
            return REFLECTANCE_NO_DATA;

        int val = (int)static_cast<float>(A[m_nPrevL3AReflectanceBandStartIndex + offset]);
        return (val != 0);
    }

    float GetPrevL3APixelFlagValue(const TInput & A)
    {
        if(!m_bPrevL3ABandsAvailable || m_nPrevL3APixelFlagBandIndex == -1)
            return FLAG_NO_DATA;

        int val = (int)static_cast<float>(A[m_nPrevL3APixelFlagBandIndex]);
        return (val != 0);
    }

    int GetBlueBandIndex()
    {
        return m_nBlueBandIndex;
    }

public:
    UpdateSynthesisFunctor()
    {
        m_fQuantificationValue = -1;
        m_bPrevL3ABandsAvailable = false;
        m_nL2ABandStartIndex = 0;
        m_nCloudMaskBandIndex = -1;
        m_nSnowMaskBandIndex = -1;
        m_nWaterMaskBandIndex = -1;
        m_nCurrentL2AWeightBandIndex = -1;
        m_nPrevL3AWeightBandStartIndex = -1;
        m_nPrevL3AWeightedAvDateBandIndex = -1;
        m_nPrevL3AReflectanceBandStartIndex = -1;
        m_nPrevL3APixelFlagBandIndex = -1;
        m_nRedBandIndex = -1;
        m_nNbOfL3AReflectanceBands = 0;
    }

    UpdateSynthesisFunctor& operator =(const UpdateSynthesisFunctor& copy)
    {
        m_fQuantificationValue = copy.m_fQuantificationValue;
        m_bPrevL3ABandsAvailable = copy.m_bPrevL3ABandsAvailable;
        m_nL2ABandStartIndex = copy.m_nL2ABandStartIndex;
        m_nCloudMaskBandIndex = copy.m_nCloudMaskBandIndex;
        m_nSnowMaskBandIndex = copy.m_nSnowMaskBandIndex;
        m_nWaterMaskBandIndex = copy.m_nWaterMaskBandIndex;
        m_nCurrentL2AWeightBandIndex = copy.m_nCurrentL2AWeightBandIndex;
        m_nPrevL3AWeightBandStartIndex = copy.m_nPrevL3AWeightBandStartIndex;
        m_nPrevL3AWeightedAvDateBandIndex = copy.m_nPrevL3AWeightedAvDateBandIndex;
        m_nPrevL3AReflectanceBandStartIndex = copy.m_nPrevL3AReflectanceBandStartIndex;
        m_nPrevL3APixelFlagBandIndex = copy.m_nPrevL3APixelFlagBandIndex;
        m_nRedBandIndex = copy.m_nRedBandIndex;
        m_nNbOfL3AReflectanceBands = copy.m_nNbOfL3AReflectanceBands;
        return *this;
    }

    bool operator!=( const UpdateSynthesisFunctor & other) const
    {
        return true;
    }
    bool operator==( const UpdateSynthesisFunctor & other ) const
    {
        return !(*this != other);
    }

    TOutput operator()( const TInput & A )
    {
        ResetCurrentPixelValues();
        if(IsLandPixel(A))
        {
            HandleLandPixel(A);
        } else {
            if(IsSnowPixel(A) || IsWaterPixel(A))
            {
                // if pixel is snow or water *replace the reflectance value
                HandleSnowOrWaterPixel(A);
            } else {
                // if pixel is cloud or shadow *pixel never observed cloud snow or water free
                HandleCloudOrShadowPixel(A);
            }
        }

        int nTotalOutBandsNo = 2*m_nNbOfL3AReflectanceBands + 2;
        TOutput var(nTotalOutBandsNo);
        var.SetSize(nTotalOutBandsNo);

        int i;
        int cnt = 0;

        // Weighted Average Reflectances
        for(i = 0; i < m_nNbOfL3AReflectanceBands; i++)
        {
            var[cnt++] = m_CurrentPixelWeights[i];
        }
        // Weighted Average Date L3A
        var[cnt++] = m_fCurrentPixelWeightedDate;
        // Weight for B2 for L3A
        for(i = 0; i < m_nNbOfL3AReflectanceBands; i++)
        {
            var[cnt++] = m_CurrentWeightedReflectances[i];
        }
        // Pixel status
        var[cnt++] = m_fCurrentPixelFlag;

        return var;
    }

    void Initialize(SensorType sensorType, ResolutionType resolution, bool bPrevL3ABandsAvailable)
    {
        m_sensorType = sensorType;
        m_resolution = resolution;
        m_bPrevL3ABandsAvailable = bPrevL3ABandsAvailable;
        if(resolution == RES_10M) {
            m_nNbOfL3AReflectanceBands = WEIGHTED_REFLECTANCE_10M_BANDS_NO;
            if(sensorType == SENSOR_S2)
            {
                m_nNbL2ABands = S2_L2A_10M_BANDS_NO;
                m_nL2ABandStartIndex = S2_L2A_10M_BANDS_START_IDX;
                m_nCloudMaskBandIndex = S2_L2A_10M_CLD_MASK_IDX;
                m_nSnowMaskBandIndex = S2_L2A_10M_SNOW_MASK_IDX;
                m_nWaterMaskBandIndex = S2_L2A_10M_WATER_MASK_IDX;
                m_nCurrentL2AWeightBandIndex = S2_L2A_10M_TOTAL_WEIGHT_IDX;
                m_nPrevL3AWeightBandStartIndex = S2_L2A_10M_L3A_WEIGHT_START_IDX;
                m_nPrevL3AWeightedAvDateBandIndex = S2_L2A_10M_L3A_W_AV_DATE_IDX;
                m_nPrevL3AReflectanceBandStartIndex = S2_L2A_10M_L3A_REFL_START_IDX;
                m_nPrevL3APixelFlagBandIndex = S2_L2A_10M_L3A_PIXEL_STATUS_IDX;
                m_nRedBandIndex = S2_L2A_10M_RED_BAND_IDX;
                m_nBlueBandIndex = S2_L2A_10M_BLUE_BAND_IDX;

            } else if (sensorType == SENSOR_L8) {

                m_nNbL2ABands = L8_L2A_10M_BANDS_NO;
                m_nL2ABandStartIndex = L8_L2A_10M_BANDS_START_IDX;
                m_nCloudMaskBandIndex = L8_L2A_10M_CLD_MASK_IDX;
                m_nSnowMaskBandIndex = L8_L2A_10M_SNOW_MASK_IDX;
                m_nWaterMaskBandIndex = L8_L2A_10M_WATER_MASK_IDX;
                m_nCurrentL2AWeightBandIndex = L8_L2A_10M_TOTAL_WEIGHT_IDX;
                m_nPrevL3AWeightBandStartIndex = L8_L2A_10M_L3A_WEIGHT_START_IDX;
                m_nPrevL3AWeightedAvDateBandIndex = L8_L2A_10M_L3A_W_AV_DATE_IDX;
                m_nPrevL3AReflectanceBandStartIndex = L8_L2A_10M_L3A_REFL_START_IDX;
                m_nPrevL3APixelFlagBandIndex = L8_L2A_10M_L3A_PIXEL_STATUS_IDX;
                m_nRedBandIndex = L8_L2A_10M_RED_BAND_IDX;
                m_nBlueBandIndex = L8_L2A_10M_BLUE_BAND_IDX;

            } else {
                // TODO: Throw an error
            }
        } else if(resolution == RES_20M) {
            m_nNbOfL3AReflectanceBands = WEIGHTED_REFLECTANCE_20M_BANDS_NO;
            if(sensorType == SENSOR_S2)
            {
                m_nNbL2ABands = S2_L2A_20M_BANDS_NO;
                m_nL2ABandStartIndex = S2_L2A_20M_BANDS_START_IDX;
                m_nCloudMaskBandIndex = S2_L2A_20M_CLD_MASK_IDX;
                m_nSnowMaskBandIndex = S2_L2A_20M_SNOW_MASK_IDX;
                m_nWaterMaskBandIndex = S2_L2A_20M_WATER_MASK_IDX;
                m_nCurrentL2AWeightBandIndex = S2_L2A_20M_TOTAL_WEIGHT_IDX;
                m_nPrevL3AWeightBandStartIndex = S2_L2A_20M_L3A_WEIGHT_START_IDX;
                m_nPrevL3AWeightedAvDateBandIndex = S2_L2A_20M_L3A_W_AV_DATE_IDX;
                m_nPrevL3AReflectanceBandStartIndex = S2_L2A_20M_L3A_REFL_START_IDX;
                m_nPrevL3APixelFlagBandIndex = S2_L2A_20M_L3A_PIXEL_STATUS_IDX;
                m_nRedBandIndex = S2_L2A_20M_RED_BAND_IDX;
                m_nBlueBandIndex = S2_L2A_20M_BLUE_BAND_IDX;

            } else if (sensorType == SENSOR_L8) {

                m_nNbL2ABands = L8_L2A_20M_BANDS_NO;
                m_nL2ABandStartIndex = L8_L2A_20M_BANDS_START_IDX;
                m_nCloudMaskBandIndex = L8_L2A_20M_CLD_MASK_IDX;
                m_nSnowMaskBandIndex = L8_L2A_20M_SNOW_MASK_IDX;
                m_nWaterMaskBandIndex = L8_L2A_20M_WATER_MASK_IDX;
                m_nCurrentL2AWeightBandIndex = L8_L2A_20M_TOTAL_WEIGHT_IDX;
                m_nPrevL3AWeightBandStartIndex = L8_L2A_20M_L3A_WEIGHT_START_IDX;
                m_nPrevL3AWeightedAvDateBandIndex = L8_L2A_20M_L3A_W_AV_DATE_IDX;
                m_nPrevL3AReflectanceBandStartIndex = L8_L2A_20M_L3A_REFL_START_IDX;
                m_nPrevL3APixelFlagBandIndex = L8_L2A_20M_L3A_PIXEL_STATUS_IDX;
                m_nRedBandIndex = L8_L2A_20M_RED_BAND_IDX;
                m_nBlueBandIndex = L8_L2A_20M_BLUE_BAND_IDX;

            } else {
                // TODO: Throw an error
            }
        } else {
            // TODO: Throw an error
        }
    }

    void SetReflectanceQuantificationValue(float fQuantifVal)
    {
        m_fQuantificationValue = fQuantifVal;
    }

    void SetCurrentDate(int nDate)
    {
        m_nCurrentDate = nDate;
    }
};

#endif // UPDATESYNTHESIS_H