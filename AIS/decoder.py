from pyais import decode

arquivoEntrada = "AIS/nmea0183.txt"
arquivoSaida = "AIS/DadosAIS.txt"

with open(arquivoEntrada, "r") as f, open(arquivoSaida, "w") as out:
    for linha in f:
        linha = linha.strip()
        if not linha:
            continue  
        try:
            mensagemDecodificada = decode(linha)
            parametros = mensagemDecodificada.asdict()
            
            for chave, valor in parametros.items():
                out.write(f"{chave}: {valor}\n")
            out.write("\n")
        
        except Exception as e:
            out.write(f"\nErro ao decodificar linha: {linha}\n")
            out.write(f"Detalhe do erro: {e}\n")
    