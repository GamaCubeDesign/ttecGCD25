import json
from pyais import decode

def limpar_bytes(obj):
    if isinstance(obj, dict):
        return {k: limpar_bytes(v) for k, v in obj.items()}
    elif isinstance(obj, list):
        return [limpar_bytes(i) for i in obj]
    elif isinstance(obj, bytes):
        return obj.hex()  # ou obj.decode('utf-8', errors='replace') 
    else:
        return obj

arquivo_entrada = "AIS/nmea0183.txt"
arquivo_saida = "AIS/saida_decodificada.jsonl"

with open(arquivo_entrada, "r") as f, open(arquivo_saida, "w") as out:
    for linha in f:
        linha = linha.strip()
        if not linha:
            continue
        try:
            mensagemDecodificada = decode(linha)
            parametros = mensagemDecodificada.asdict()
            parametros = limpar_bytes(parametros)

            json.dump(parametros, out, ensure_ascii=False, indent=2)
            out.write('\n\n')
        except Exception as e:
            erro = {
                "erro": str(e),
                "linha": linha
            }
            json.dump(erro, out, ensure_ascii=False, indent=2)
            out.write('\n\n')
