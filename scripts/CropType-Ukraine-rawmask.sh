#!/bin/bash

./CropType.py -ref /mnt/Imagery_S2A/In-Situ_TDS/Ukraine/LC/UA_KYIV_LC_FO_2013.shp -ratio 0.75 -input \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130206_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130206_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130226_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130226_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130318_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130318_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130402_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130402_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130412_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130412_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130417_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130417_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130422_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130422_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130427_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130427_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130502_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130502_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130507_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130507_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130512_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130512_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130517_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130517_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130527_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130527_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130601_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130601_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130606_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130606_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130611_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130611_N2A_EUkraineD0000B0000.xml \
/mnt/Imagery_S2A/L2A/Spot4-T5/Ukraine/SPOT4_HRVIR1_XS_20130616_N2A_EUkraineD0000B0000/SPOT4_HRVIR1_XS_20130616_N2A_EUkraineD0000B0000.xml \
-t0 20130206 -tend 20130616 -rate 5 -radius 100 -classifier rf -rseed 0 -mask /mnt/data/ukraine/Ukraine-mask/raw_crop_mask.tif -pixsize 20 \
-outdir /mnt/data/ukraine/Ukraine-raw
