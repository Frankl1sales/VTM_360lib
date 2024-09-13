# VTM-23.4 com 360Lib Integração

Este projeto utiliza o software **VTM-23.4** integrado com a biblioteca **360Lib** para realizar codificações de vídeos em domínios esféricos. Abaixo estão os passos para preparar, compilar e executar o software.
## Expert

| [<img src="https://avatars.githubusercontent.com/u/114530594?s=400&u=3efdade63f17708f59dc3d0e96545a108a0147ef&v=4" width="150px;"/>](https://github.com/Frankl1sales) |
| :-----------------------------------------------------------------------------------------------------------------: |
|                                 [Franklin Sales](https://github.com/Frankl1sales)                                  |

## 1. Requisitos

- 360Lib: [Acesse aqui](https://vcgit.hhi.fraunhofer.de/jvet/360lib)
- VTM na versão 23.4: [Baixe aqui](https://vcgit.hhi.fraunhofer.de/jvet/VVCSoftware_VTM)

## 2. Preparação do Software

### 2.1 Obtenha os arquivos necessários

Faça o checkout dos repositórios **360Lib** e **VTM-23.4**:

```bash
git clone https://vcgit.hhi.fraunhofer.de/jvet/360lib
git clone https://vcgit.hhi.fraunhofer.de/jvet/VVCSoftware_VTM -b VTM-23.4
```

### 2.2 Copie os arquivos da 360Lib para o VTM

Após clonar os repositórios, é necessário copiar alguns arquivos da **360Lib** para o diretório do **VTM-23.4**.

#### 2.2.1 Copie os arquivos de código-fonte:

```bash
cp -r ./360Lib/source/Lib/Lib360 ./VTM-23.4/source/Lib/
cp -r ./360Lib/source/Lib/AppEncHelper360 ./VTM-23.4/source/Lib/
cp -r ./360Lib/source/App/utils/App360Convert ./VTM-23.4/source/App/utils/
```

#### 2.2.2 Copie os arquivos de configuração:

```bash
cp -r ./360Lib/cfg-360Lib ./VTM-23.4/
```

## 3. Compilação

### 3.1 Crie o diretório de compilação

Dentro do diretório do **VTM-23.4**, crie uma pasta `build`:

```bash
mkdir build
cd build
```

### 3.2 Execute o CMake

Use o CMake para gerar os arquivos de compilação com o suporte para vídeos em 360°:

```bash
cmake .. -DEXTENSION_360_VIDEO=1 -DCMAKE_BUILD_TYPE=Release
```

### 3.3 Compile o projeto

Execute o comando `make` para compilar:

```bash
make
```

**Nota:** Em algumas máquinas, pode ocorrer erro ao utilizar `make -j`. Neste caso, utilize o comando `make` sem a opção `-j`.

## 4. Execução

Com a compilação bem-sucedida, agora seu sistema está pronto para codificar vídeos usando o **VTM** com a **360Lib**.

### 4.1 Prepare a pasta de vídeos

Crie o diretório `test_seq` no diretório raiz do **VTM-23.4** e adicione os vídeos no formato `.yuv`:

```bash
mkdir test_seq
# Copie seus arquivos .yuv para a pasta test_seq
```

### 4.2 Execute o codificador

Para codificar um vídeo, utilize o comando abaixo no diretório raiz do **VTM-23.4**. Substitua `NOME-DO-VÍDEO-YUV` pelo nome do arquivo `.yuv` que você deseja codificar.

```bash
./bin/EncoderAppStatic -c ./cfg/encoder_randomaccess_vtm.cfg \
-c ./cfg-360Lib/encoder_360_GCMP.cfg \
-c ./cfg-360Lib/per-sequence/360/360test_Trolley.cfg \
-c ./cfg-360Lib/per-sequence/360/360test_Trolley_DynamicViewports.cfg \
--SphFile=./cfg-360Lib/360Lib/sphere_655362.txt \
-i ./test_seq/NOME-DO-VÍDEO-YUV \
--CodingFaceWidth=1280 --CodingFaceHeight=1280 \
--IntraPeriod=32 -b test.bin --SEIDecodedPictureHash=1 \
--PrintHexPSNR=1 -o '' -q 27
```

## 5. Observações

- Este README foi adaptado devido a problemas encontrados com a versão **VTM-19.0**, encontrada no TCC de Otávio. Por isso, optou-se pela versão **VTM-23.4**.

- Caso tenha problemas na execução do `make -j`, utilize apenas `make`.

Agora o sistema está pronto para codificar vídeos em 360° com a biblioteca 360Lib.

### Explicações Adicionais:
- **Configurações do CMake**: O parâmetro `-DEXTENSION_360_VIDEO=1` ativa as extensões para codificação de vídeos em 360°.
- **Make**: O comando `make` é usado para compilar o software gerado pelo CMake.
- **Comando de codificação**: Ele inclui o uso de vários arquivos de configuração específicos do VTM e 360Lib, com parâmetros que definem a resolução, o período intra (32 quadros), e outros aspectos da codificação, como a métrica PSNR. 

