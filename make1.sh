#!/bin/bash
CURDIR="$(pwd)"

#cd "${CURDIR}/u-boot" && make distclean  && make nw5061_v10_zImage_sfc_nand && make && \
#cd "${CURDIR}/kernel-3.0.8" && make distclean && make jf_defconfig && make && \
#cd "${CURDIR}/mozart" && make distclean  && make jf_ap6212_ubifs_config && make 

#cd "${CURDIR}/kernel-3.0.8" && make distclean && make nw5061_oss_nand_ubi_defconfig && make && \
#cd "${CURDIR}/mozart" && make distclean  && make nw5061_ap6212_ubifs_config && make && \ 
#cd "${CURDIR}/u-boot" && make distclean  && make nw5061_v10_zImage_sfc_nand && make

cd "${CURDIR}/kernel-3.0.8" && make distclean && make lb_oss_nand_ubi_defconfig && make && \
cd "${CURDIR}/mozart" && make distclean  && make nw5027lb_ap6212_ubifs_config && make && \ 
cd "${CURDIR}/u-boot" && make distclean  && make canna_v10_zImage_sfc_nand_64mb_lpddr && make

#cd "${CURDIR}/u-boot" && make distclean  && make nw5061_v10_zImage_gpt_msc0 && make
#cd "${CURDIR}/kernel-3.0.8" && make distclean && make nw5061_oss_mmc0_defconfig && make
#cd "${CURDIR}/mozart" && make distclean  && make nw5061_ap6212_ext4_config && make 
